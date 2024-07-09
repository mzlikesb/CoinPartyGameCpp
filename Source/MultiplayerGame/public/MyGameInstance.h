// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubSystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MyGameInstance.generated.h"

/**
 *
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FonFoundSessions);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FonFoundSession, FString, SessionName, int32, MaxPlayers, int32, CurrentPlayers, int32, SessionIndex);

UCLASS()
class MULTIPLAYERGAME_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	void CreateGame(FString roomName);

	FText GetRoomName();

	void FindGame();
	void JoinGame(int32 sessionIndex);
	void EndGame();
	void RestartGame();

	void SaveData();
	void LoadData();

	UPROPERTY()
	FonFoundSession onFoundSession;

	UPROPERTY()
	FonFoundSessions onFoundSessions;

protected:
	virtual void Shutdown() override;

private:

	void OnCreatedSession(FName sessionName, bool bWasSuccessful);
	void OnDestroyedSession(FName sessionName, bool bWasSuccessful);
	void OnFoundSessions(bool bWasSuccessful);
	void OnJoinedSession(FName sessionName, EOnJoinSessionCompleteResult::Type result);

	IOnlineSessionPtr sessionInterface;
	FName currentSessionName;
	FName currentRoomName;
	TSharedPtr<FOnlineSessionSettings> sessionSettings;
	TSharedPtr<FOnlineSessionSearch> sessionSearch;

};
