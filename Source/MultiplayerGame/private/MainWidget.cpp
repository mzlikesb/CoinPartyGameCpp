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
#include "ButtonWidget.h"
#include "Components/VerticalBox.h"

void UMainWidget::NativeConstruct() {
    Super::NativeConstruct();

    CreateButton->OnClicked.AddDynamic(this, &UMainWidget::CreateGame);
    FindButton->OnClicked.AddDynamic(this, &UMainWidget::FindSessions);
    ExitButton->OnClicked.AddDynamic(this, &UMainWidget::Exit);
    
    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (GI) {

        GI->onFoundSession.AddDynamic(this, &UMainWidget::OnFoundSession);
        GI->onFoundSessions.AddDynamic(this, &UMainWidget::OnFoundSessions);
    }
}

void UMainWidget::CreateGame(){
    CreateButton->SetIsEnabled(false);

    if (PlayerName->GetText().IsEmpty()) return;

    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    
    if(GI){
        GI->CreateGame("MyGame");
    }
}

void UMainWidget::FindSessions() {
    FindButton->SetIsEnabled(false);
    SessionList->ClearChildren();
    
    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (GI) {
        GI->FindGame();
    }
}

void UMainWidget::OnFoundSession(FString SessionName, int32 MaxPlayers, int32 CurrentPlayers, int32 SessionIndex) {
    UE_LOG(LogTemp, Error, TEXT(" %s"), *SessionName);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(" %s"), *SessionName));
    }
    if (!ButtonWidget) return;

    UButtonWidget* Button = CreateWidget<UButtonWidget>(GetWorld(), ButtonWidget);
    FString Label = FString::Printf(TEXT(" %s ( %d / %d )"), *SessionName.Left(6), CurrentPlayers, MaxPlayers);
    Button->Init(FText::FromString(Label), SessionIndex);
    Button->onClick.AddDynamic(this, &UMainWidget::JoinSessions);
    SessionList->AddChild(Button);
}

void UMainWidget::OnFoundSessions() {
    FindButton->SetIsEnabled(true);
}

void UMainWidget::JoinSessions(int32 SessionIndex) {
    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (GI) {
        GI->JoinGame(SessionIndex);
    }
}

void UMainWidget::Exit() {
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