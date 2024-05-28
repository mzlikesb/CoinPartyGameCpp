// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
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
            SessionSettings->bIsLANMatch = true;
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

FName UMyGameInstance::GetSessionName(){
    return CurrentSessionName;
}