// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MyCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACoin::ACoin()
{
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACoin::OnBeginOverlapComponentEvent);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Sphere);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetGenerateOverlapEvents(false);

	RotatingComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
	
	bReplicates = true;
}

// Called when the game starts or when spawned
void ACoin::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority()) {
		InitCoin();
	}
}

// Called every frame
void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACoin::InitCoin_Implementation() {
	int CoinRate = FMath::RandRange(1, 10);

	if (CoinRate < 5) {
		CoinValue = 1;
	}
	else if (CoinRate < 9) {
		CoinValue = 5;
	}
	else {
		CoinValue = 10;
	}
	OnRep_CoinValue();
}

void ACoin::OnRep_CoinValue()
{
	if (CoinValue < 5) {
		Mesh->SetMaterial(0, BronzeMaterial);
	}
	else if (CoinValue < 9) {
		Mesh->SetMaterial(0, SilverMaterial);
	}
	else {
		Mesh->SetMaterial(0, GoldMaterial);
	}
}

void ACoin::DestroyCoin_Implementation() {
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//spawn sound at location
	//spwan emitter at location
	// hidden actor 
}

void ACoin::OnBeginOverlapComponentEvent(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult
) {
	if (!HasAuthority()) return;

	if (AMyCharacter* pawn = Cast<AMyCharacter>(OtherActor)) {
		//pawn->AddCoin(CoinValue);
		DestroyCoin();
	}
}

void ACoin::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACoin, CoinValue);
}