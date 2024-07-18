// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyPlayerController.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERGAME_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AMyGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION()
	void UpdateAllPlayerData();

	UPROPERTY(VisibleAnywhere)
	TArray<FPlayerData> AllPlayerData;

	UPROPERTY(VisibleAnywhere)
	TArray<AMyPlayerController*> Players;

	void Logout(AController* Exiting) override;
};
