// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Hat = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hat"));
	Hat->SetupAttachment(GetMesh(), TEXT("head"));

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UMyGameInstance* gi = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (gi && GetController()->IsLocalPlayerController()) {
		SetPlayerHat(gi->GetPlayerHatType());
		SetPlayerColor(gi->GetPlayerColor());
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyCharacter::SetPlayerHat_Implementation(EHatType type) {
	if (!Hat) return;

	FString path = "";

	switch (type)
	{
	case EHatType::NoHat:
		path = "";
		break;
	case EHatType::Apple:
		path = TEXT("/Game/Assets/Models/apple.apple");
		break;
	case EHatType::Banana:
		path = TEXT("/Game/Assets/Models/banana.banana");
		break;
	case EHatType::Carrot:
		path = TEXT("/Game/Assets/Models/carrot.carrot"); 
		break;
	default:
		path = "";
		break;
	}

	if (path.IsEmpty()) {
		Hat->SetStaticMesh(nullptr);
	}
	else {
		Hat->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *path));
	}
}		

void AMyCharacter::SetPlayerColor_Implementation(FLinearColor color) {
	USkeletalMeshComponent* skeletalMeshComp = GetMesh();
	if (!skeletalMeshComp)return;

	int materialNum = skeletalMeshComp->GetMaterials().Num();
	for (int i = 0; i < materialNum; i++)
	{
		UMaterialInstanceDynamic* dynamicMaterial = skeletalMeshComp->CreateAndSetMaterialInstanceDynamic(i);
		if (dynamicMaterial) {
			dynamicMaterial->SetVectorParameterValue(FName("Tint"), color);
		}
	}
}