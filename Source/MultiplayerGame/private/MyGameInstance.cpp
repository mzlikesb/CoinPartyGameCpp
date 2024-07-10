// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void UMyGameInstance::Init()
{
    Super::Init();

    sessionInterface = IOnlineSubsystem::Get()->GetSessionInterface();

    if (sessionInterface != nullptr)
    {
        sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnCreatedSession);
        sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMyGameInstance::OnFoundSessions);
        sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnJoinedSession);
        sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnDestroyedSession);
    }
}

void UMyGameInstance::CreateGame(FString roomName) {

    //https://unrealcommunity.wiki/online-multiplayer-vkje2zyn

    sessionSettings = MakeShareable(new FOnlineSessionSettings());
    sessionSettings->NumPrivateConnections = 0;
    sessionSettings->NumPublicConnections = 4;
    sessionSettings->bAllowInvites = true;
    sessionSettings->bAllowJoinInProgress = true;
    sessionSettings->bAllowJoinViaPresence = true;
    sessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
    sessionSettings->bIsDedicated = false;
    sessionSettings->bUsesPresence = true;
    sessionSettings->bIsLANMatch = true;
    sessionSettings->bShouldAdvertise = true;

    currentSessionName = NAME_GameSession;
    currentRoomName = FName(*roomName);

    sessionSettings->Set(FName("ROOM_NAME"), roomName, EOnlineDataAdvertisementType::ViaOnlineService);

    const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();

    sessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *sessionSettings);

}

void UMyGameInstance::EndGame() {
    sessionInterface->DestroySession(currentSessionName);
}

void UMyGameInstance::Shutdown() {
    Super::Shutdown();

    UE_LOG(LogTemp, Log, TEXT("GameInstance is shutting down."));
    UMyGameInstance::EndGame();
}

void UMyGameInstance::OnCreatedSession(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        UE_LOG(LogTemp, Log, TEXT("Session created successfully! : %s"), *SessionName.ToString());

        GetWorld()->ServerTravel("Game?Listen", true);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create session."));
    }
}

void UMyGameInstance::OnDestroyedSession(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        UGameplayStatics::OpenLevel(GetWorld(), FName("Menu"));
    }
}

FText UMyGameInstance::GetRoomName() {
    return FText::FromName(currentRoomName);
}

void UMyGameInstance::FindGame() {

    sessionSearch = MakeShareable(new FOnlineSessionSearch());
    sessionSearch->bIsLanQuery = true;
    sessionSearch->MaxSearchResults = 20;
    sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Type::Equals);//EOnlineComparisonOp::Equals);

    if (sessionInterface->FindSessions(0, sessionSearch.ToSharedRef()))
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Finding sessions..."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Fail to find session"));
    }
}

void UMyGameInstance::OnFoundSessions(bool bWasSuccessful) {
    TArray<FOnlineSessionSearchResult> results = sessionSearch->SearchResults;

    if (!bWasSuccessful) return;

    int32 sessionNum = results.Num();
    UE_LOG(LogTemp, Log, TEXT("Find Sessions %d."), sessionNum);

    for (int32 i = 0; i < sessionNum; i++)
    {
        FString roomName;
        results[i].Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
        int32 maxPlayerNumber = results[i].Session.SessionSettings.NumPublicConnections;
        int32 currentPlayerNumber = maxPlayerNumber - results[i].Session.NumOpenPublicConnections;
        onFoundSession.Broadcast(roomName, maxPlayerNumber, currentPlayerNumber, i);
    }

    onFoundSessions.Broadcast();
}
void UMyGameInstance::JoinGame(int32 SessionIndex) {

    FString roomName;
    sessionSearch->SearchResults[SessionIndex].Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
    
    currentRoomName = FName(*roomName);
    
    if (sessionInterface->JoinSession(0, FName(*roomName), sessionSearch->SearchResults[SessionIndex])) {

        UE_LOG(LogTemp, Error, TEXT("join session %s ..."), *roomName);
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("Fail to join %s ..."), *roomName);
    }
}

void UMyGameInstance::OnJoinedSession(FName sessionName, EOnJoinSessionCompleteResult::Type result) {
    
    UE_LOG(LogTemp, Error, TEXT("join session %s"), *sessionName.ToString());

    currentSessionName = sessionName;

    switch (result)
    {
    case EOnJoinSessionCompleteResult::Success:
    {
        UE_LOG(LogTemp, Warning, TEXT("Join Success!"));

        APlayerController* pc = GetWorld()->GetFirstPlayerController();
        FString url;
        sessionInterface->GetResolvedConnectString(sessionName, url, NAME_GamePort);
        UE_LOG(LogTemp, Warning, TEXT("url: %s"), *url);

        pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);

        break;
    }
    case EOnJoinSessionCompleteResult::SessionIsFull:
    {
        UE_LOG(LogTemp, Warning, TEXT("Session is Full"));
        break;
    }
    default:
        UE_LOG(LogTemp, Warning, TEXT("error"));
        break;
    }
}

void UMyGameInstance::SetPlayerName(FString name) { PlayerData.Name = name; }

FString UMyGameInstance::GetPlayerName() { return PlayerData.Name; }

void UMyGameInstance::SetPlayerHatType(EHatType type) { PlayerData.HatType = type; }

EHatType UMyGameInstance::GetPlayerHatType() { return PlayerData.HatType; }

void UMyGameInstance::SetPlayerColor(FLinearColor color) { PlayerData.Color = color; }

FLinearColor UMyGameInstance::GetPlayerColor() { return PlayerData.Color; }