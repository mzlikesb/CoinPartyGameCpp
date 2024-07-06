// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"


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