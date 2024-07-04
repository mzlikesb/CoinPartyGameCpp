// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERGAME_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	int a;

	UFUNCTION(BlueprintCallable)
	void CreateGame(FName SessionName);

	UFUNCTION(BlueprintCallable)
	void FindSessions();

	UFUNCTION(BlueprintCallable)
	void JoinSessions(int32 SessionIndex);


protected:

	void OnFoundSessions(FString SessionName, int32 MaxPlayers, int32 CurrentPlayers, int32 SessionIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateSlots(FName SessionName, int32 MaxPlayers, int32 CurrentPlayers, int32 SessionIndex);

};