// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "MyPlayerController.h"
#include "MyCharacter.h"

AMyGameMode::AMyGameMode()
{
	
}

void AMyGameMode::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	
	AMyPlayerController* pc = Cast<AMyPlayerController>(NewPlayer);
	if (pc) {
		pc->RequestClientToSendPlayerData();
		Players.Add(pc);
	}
}

void AMyGameMode::UpdateAllPlayerData() {
	
	AllPlayerData.Empty();

	for (int i = 0; i < Players.Num(); i++) {
		AllPlayerData.Add(Players[i]->PlayerData);
		APawn* pawn = Players[i]->GetPawn();
		if (pawn) {
			AMyCharacter* character = Cast<AMyCharacter>(pawn);
			character->SetPlayerHat(AllPlayerData[i].HatType);
			character->SetPlayerColor(AllPlayerData[i].Color);
		}
	}
	for (AMyPlayerController* player : Players) {
		player->UpdateAllPlayerData(AllPlayerData);
	}
}

void AMyGameMode::Logout(AController* Exiting) {
	Super::Logout(Exiting);
	UE_LOG(LogTemp, Error, TEXT("Exit Player"));

	AMyPlayerController* player = Cast<AMyPlayerController>(Exiting);
	if (player) {
		
		int32 index = Players.Find(player);
		Players.RemoveAt(index);
		AllPlayerData.RemoveAt(index);
		UpdateAllPlayerData();
	}
}