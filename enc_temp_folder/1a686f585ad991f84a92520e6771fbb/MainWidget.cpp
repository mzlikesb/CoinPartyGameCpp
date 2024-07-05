// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"
#include "Components/EditableText.h"

void UMainWidget::NativeConstruct() {
    Super::NativeConstruct();

    CreateButton->OnClicked.AddDynamic(this, &UMainWidget::CreateGame);
    FindButton->OnClicked.AddDynamic(this, &UMainWidget::FindSessions);
    ExitButton->OnClicked.AddDynamic(this, &UMainWidget::OnClickExitButton);

}

void UMainWidget::CreateGame(){
    if (GameName->GetText().IsEmpty()) return;

    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    
    if(GI){
        FName SessionName = FName(GameName->Text.ToString());
        GI->CreateGame(SessionName);
    }
}

void UMainWidget::FindSessions() {

    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (GI) {

        GI->onFoundSessions.AddDynamic(this, &UMainWidget::OnFoundSessions);
        GI->FindGame();
    }
}

void UMainWidget::OnFoundSessions(FString SessionName, int32 MaxPlayers, int32 CurrentPlayers, int32 SessionIndex) {
    
}

void UMainWidget::JoinSessions(int32 SessionIndex) {
    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (GI) {
        GI->JoinGame(SessionIndex);
    }
}

void UMainWidget::OnClickExitButton() {
    UWorld* World = GetWorld();
    if (World)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
        if (PlayerController)
        {
            PlayerController->ConsoleCommand("quit");
        }
    }
}