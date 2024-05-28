// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubSystem.h"
#include "OnlineSessionSettings.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERGAME_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void CreateGame(FName SessionName);

	UFUNCTION(BlueprintCallable)
	FName GetSessionName();

	void FindGame();
	void JoinGame();
	void EndGame();
	void RestartGame();

	void SaveData();
	void LoadData();


protected:
    virtual void Shutdown() override;

private:
    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

    TSharedPtr<FOnlineSessionSettings> SessionSettings;
	FName CurrentSessionName;

};
