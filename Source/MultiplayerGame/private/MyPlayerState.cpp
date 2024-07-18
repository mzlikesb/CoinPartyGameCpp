// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "MyPlayerController.h"

void AMyPlayerState::AddCoins(uint8 Value) {
	Coins += Value;
	AMyPlayerController* pc = Cast<AMyPlayerController>(GetPlayerController());
	if (pc) {
		pc->UpdateCoin(Coins);
	}
}