// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "MyPlayerController.h"
#include "PlayerScore.h"

void AMyPlayerState::AddCoins(uint8 Value) {
	PlayerScore.Coins += Value;
	AMyPlayerController* pc = Cast<AMyPlayerController>(GetPlayerController());
	if (pc) {
		pc->UpdateCoin(PlayerScore.Coins);
	}
}

void AMyPlayerState::InitScore(uint8 ID, FString Name) {
	PlayerScore.PlayerID = ID;
	PlayerScore.Name = Name;
}