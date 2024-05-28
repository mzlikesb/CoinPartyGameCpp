// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"


void AMyPlayerController::BeginPlay(){

    Super::BeginPlay();
    SetWidget();
}

void AMyPlayerController::SetWidget(){
    
    if(WidgetClass){
        UUserWidget* Widget = CreateWidget<UUserWidget>(this, WidgetClass);
        if(Widget){
            Widget->AddToViewport();
        }
    }
}