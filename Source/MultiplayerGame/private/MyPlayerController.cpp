// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "MyGameMode.h"
#include "MyCharacter.h"
#include "GameWidget.h"


void AMyPlayerController::BeginPlay(){

    Super::BeginPlay();
    SetWidget();
}

void AMyPlayerController::SetWidget(){
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (PC && PC->IsLocalController()){
        if (WidgetClass) {
            UUserWidget* Widget = CreateWidget<UUserWidget>(this, WidgetClass);
            if (Widget) {
                Widget->AddToViewport();
                bShowMouseCursor = true;
                MainWidget = Widget;
            }
        }
    }
}

void AMyPlayerController::RequestClientToSendPlayerData_Implementation() {

    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    ServerReceivePlayerData(GI->PlayerData);
}

void AMyPlayerController::ServerReceivePlayerData_Implementation(const FPlayerData& ReceivedPlayerData)
{
   AMyGameMode* GM = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

   this->PlayerData = ReceivedPlayerData;
   GM->ReceivePlayerData(ReceivedPlayerData);
}


void AMyPlayerController::OnPossess(APawn* InPawn) {

    Super::OnPossess(InPawn);

}

void AMyPlayerController::SendChat_Implementation(const FText& Text) {
    AMyGameMode* GM = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    FString ChatText = PlayerData.Name + FString(": ") + Text.ToString();
    for (AMyPlayerController* player : GM->Players) {
        player->ReceiveChat(FText::FromString(*ChatText));
    }
}

void AMyPlayerController::ReceiveChat_Implementation(const FText& Text) {
    if (!MainWidget) return;

    UGameWidget* Widget = Cast<UGameWidget>(MainWidget);
    if (Widget) {
        Widget->ReceiveChat(Text);
    }
}