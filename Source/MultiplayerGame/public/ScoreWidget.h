// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerScore.h"
#include "ScoreWidget.generated.h"

UCLASS()
class MULTIPLAYERGAME_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* PlayerName;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* PlayerCoins;

	UFUNCTION()
	void SetScore(FPlayerScore Score);
};
