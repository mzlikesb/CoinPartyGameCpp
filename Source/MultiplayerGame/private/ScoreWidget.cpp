// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreWidget.h"
#include "Components/TextBlock.h"

void UScoreWidget::SetScore(FPlayerScore Score) {
	PlayerName->SetText(FText::FromString(Score.Name));
	PlayerCoins->SetText(FText::AsNumber(Score.Coins));
}