// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "MyGameMode.h"
#include "MyCharacter.h"


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
            }
        }
    }
}

void AMyPlayerController::RequestClientToSendPlayerData_Implementation() {

    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    ServerReceivePlayerData(GI->PlayerData);
}

void AMyPlayerController::ServerReceivePlayerData_Implementation(const FPlayerData& playerData)
{
   AMyGameMode* gm = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

   this->PlayerData = playerData;
   gm->ReceivePlayerData(playerData);
}


void AMyPlayerController::OnPossess(APawn* InPawn) {

    Super::OnPossess(InPawn);

}