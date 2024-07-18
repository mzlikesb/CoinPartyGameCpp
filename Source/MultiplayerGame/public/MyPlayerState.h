// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
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
	uint8 Coins;

public:
	UFUNCTION()
	void AddCoins(uint8 Value);


};
