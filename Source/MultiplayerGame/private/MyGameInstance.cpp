// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

void UMyGameInstance::Init()
{
	Super::Init();
}

void UMyGameInstance::CreateGame(){
    
    /*
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (!OnlineSubsystem)return;
    IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
    if (!Sessions.IsValid())return;
    */

    //https://unrealcommunity.wiki/online-multiplayer-vkje2zyn

    const IOnlineSessionPtr Sessions = Online::GetSessionInterface(GetWorld());

    if (!Sessions.IsValid())
    {
        return;
    }

    SessionSettings = MakeShareable(new FOnlineSessionSettings());
    SessionSettings->NumPrivateConnections = 0;
    SessionSettings->NumPublicConnections = 4;
    SessionSettings->bAllowInvites = true;
    SessionSettings->bAllowJoinInProgress = true;
    SessionSettings->bAllowJoinViaPresence = true;
    SessionSettings->bAllowJoinViaPresenceFriendsOnly = true;
    SessionSettings->bIsDedicated = false;
    SessionSettings->bUsesPresence = true;
    SessionSettings->bIsLANMatch = true;
    SessionSettings->bShouldAdvertise = true;

    const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();

    Sessions->OnCreateSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnCreateSessionComplete);
    Sessions->CreateSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings); 

}

void UMyGameInstance::EndGame(){

    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    
    if(OnlineSubsystem){

        IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
        if (Sessions.IsValid())
        {
            if (Sessions->GetNamedSession(CurrentSessionName) != nullptr)
            {
                Sessions->DestroySession(CurrentSessionName,FOnDestroySessionCompleteDelegate::CreateUObject(this, &UMyGameInstance::OnDestroySessionComplete));
            }
        }
    }
}

void UMyGameInstance::Shutdown(){
    Super::Shutdown();
    
    UE_LOG(LogTemp, Log, TEXT("GameInstance is shutting down."));
    UMyGameInstance::EndGame();
}

void UMyGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        UE_LOG(LogTemp, Log, TEXT("Session created successfully! : %s"), *SessionName.ToString());
        CurrentSessionName = SessionName;
        
        const IOnlineSessionPtr sessionInterface = Online::GetSessionInterface(GetWorld());

        if (!sessionInterface.IsValid())
        {
            return;
        }
        //sessionInterface->StartSession(NAME_GameSession);

        UGameplayStatics::OpenLevel(GetWorld(), FName("Game?Listen"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create session."));
    }
}

void UMyGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        UE_LOG(LogTemp, Log, TEXT("Session destroyed"));
    }
}

FText UMyGameInstance::GetSessionName(){
    return FText::FromName(CurrentSessionName);
}

void UMyGameInstance::FindGame() {
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

    if (OnlineSubsystem) {

        IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
        if (Sessions.IsValid())
        {

            OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(
                FOnFindSessionsCompleteDelegate::CreateUObject(this, &UMyGameInstance::OnFindSessionsComplete)
            );

            SessionSearch = MakeShareable(new FOnlineSessionSearch());
            SessionSearch->bIsLanQuery = true;
            SessionSearch->MaxSearchResults = 20;
            SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

            //Sessions->FindSessions(0, SessionSearch.ToSharedRef());
            const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
            if (Sessions->FindSessions(*localPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef()))
            {
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Finding sessions..."));
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to start session search"));
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to start finding sessions."));
                }
            }
        }
    }
}

void UMyGameInstance::OnFindSessionsComplete(bool bWasSuccessful) {
    
    onFoundSessions.Broadcast();
    
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem)
    {
        IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
        if (Sessions.IsValid())
        {
            Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);      
        }
    }

    if (!bWasSuccessful)
    {
        UE_LOG(LogTemp, Log, TEXT("Find Sessions failed."));
        return;
    }

    TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
    UE_LOG(LogTemp, Log, TEXT("Find Sessions %d."), SearchResults.Num());
    if (GEngine)
    {
        FString DebugMessage = FString::Printf(TEXT("Find Sessions %d."), SearchResults.Num());
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugMessage);
    }
    int32 SessionIndex = 0;
    for(const FOnlineSessionSearchResult result : SearchResults) {
        FString SessionName = result.Session.OwningUserName;
        int32 MaxPlayerNumber = result.Session.SessionSettings.NumPublicConnections;
        int32 CurrentPlayerNumber = result.Session.NumOpenPublicConnections;
        onFoundSession.Broadcast(SessionName, MaxPlayerNumber, CurrentPlayerNumber, SessionIndex++);
    }

}
void UMyGameInstance::JoinGame(int32 SessionIndex) {
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem)
    {
        IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
        if (Sessions.IsValid())
        {
            const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
            if (Sessions->JoinSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionSearch->SearchResults[SessionIndex])) {

                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Join session..."));
                }
            }
            else {
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Fail to Join session"));
                }
            }
        }
    }
}
