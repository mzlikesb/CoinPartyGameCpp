// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"
#include "MyCharacter.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

void AMenuGameMode::BeginPlay() {
	Super::BeginPlay();

	APlayerController* localPlayer = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	AMyCharacter* character = Cast<AMyCharacter>(localPlayer->GetPawn());
	if (GI && character) {
		character->SetPlayerHat(GI->GetPlayerHatType());
		character->SetPlayerColor(GI->GetPlayerColor());

		UE_LOG(LogTemp, Warning, TEXT("Set Player"));
	}

}