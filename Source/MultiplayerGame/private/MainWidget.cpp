// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

void UMainWidget::CreateGame(FName SessionName){
    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    
    if(GI){
        GI->CreateGame(SessionName);
        GI->onFoundSessions.AddDynamic(this, &UMainWidget::OnFoundSessions);
    }
}

void UMainWidget::FindSessions() {

    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (GI) {
        GI->FindGame();
    }
}

void UMainWidget::OnFoundSessions(FString SessionName, int32 MaxPlayers, int32 CurrentPlayers, int32 SessionIndex) {
    CreateSlots(FName(*SessionName), MaxPlayers, CurrentPlayers, SessionIndex);
}

void UMainWidget::JoinSessions(int32 SessionIndex) {
    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (GI) {
        GI->JoinGame(SessionIndex);
    }
}