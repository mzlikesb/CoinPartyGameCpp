// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerScore.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERGAME_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	FPlayerScore PlayerScore = FPlayerScore();

public:
	UFUNCTION()
	void AddCoins(uint8 Value);

	UFUNCTION()
	void InitScore(uint8 ID, FString Name);

	FORCEINLINE FPlayerScore GetPlayerScore() { return PlayerScore; }

};
