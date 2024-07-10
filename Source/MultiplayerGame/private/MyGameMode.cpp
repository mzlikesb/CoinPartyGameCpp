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

void AMyGameMode::ReceivePlayerData(const FPlayerData& playerdata) {
	
	PlayerDatas.Empty();

	for (int i = 0; i < Players.Num(); i++) {
		PlayerDatas.Add(Players[i]->PlayerData);
		APawn* pawn = Players[i]->GetPawn();
		if (pawn) {
			AMyCharacter* character = Cast<AMyCharacter>(pawn);
			character->SetPlayerHat(PlayerDatas[i].HatType);
			character->SetPlayerColor(PlayerDatas[i].Color);
		}
	}
}