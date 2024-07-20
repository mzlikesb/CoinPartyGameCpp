// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PlayerScore.h"
#include "MyGameState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERGAME_API AMyGameState : public AGameState
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void StartGame();
	
	UFUNCTION()
	void GameOver();

	UFUNCTION()
	void OnTimerTick();
	
	FORCEINLINE float GetProgress() { return CurrentTime / LimitTime; }

protected:

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimerInterval = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LimitTime = 10.0f;
	
	UPROPERTY(Replicated)
	float CurrentTime;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
