// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

void UMyGameInstance::Init()
{
	Super::Init();
}

void UMyGameInstance::CreateGame(FName SessionName){
    
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    
    if(OnlineSubsystem){

        IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
        if (Sessions.IsValid())
        {
            SessionSettings = MakeShareable(new FOnlineSessionSettings());
            SessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
            SessionSettings->NumPublicConnections = 4; 
            SessionSettings->bShouldAdvertise = true;
            SessionSettings->bUsesPresence = true;

            Sessions->OnCreateSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnCreateSessionComplete);
            Sessions->CreateSession(0, SessionName, *SessionSettings);
        }
    }
    
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
        UGameplayStatics::OpenLevel(GetWorld(), FName("Game"));
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
            SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
            SessionSearch->MaxSearchResults = 20;

            Sessions->FindSessions(0, SessionSearch.ToSharedRef());
        }
    }
}

void UMyGameInstance::OnFindSessionsComplete(bool bWasSuccessful) {
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem)
    {
        IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
        if (Sessions.IsValid())
        {
            // ????? ??
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
        //onFoundSessions.Broadcast(SessionName, MaxPlayerNumber, CurrentPlayerNumber, SessionIndex++);
    }

}
void UMyGameInstance::JoinGame(int32 SessionIndex) {

}