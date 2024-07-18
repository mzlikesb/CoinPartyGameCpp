
#include "CoinSpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Coin.h"

ACoinSpawner::ACoinSpawner()
{
 	PrimaryActorTick.bCanEverTick = false;
	
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void ACoinSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority()) {
		for (uint8 i = 0; i < NumberOfCoins; i++)
		{
			FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(
				Box->GetComponentLocation(),
				Box->GetScaledBoxExtent()
			);
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			GetWorld()->SpawnActor<ACoin>(CoinClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

		}
	}
}
