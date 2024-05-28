// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UMainWidget::CreateGame(FName SessionName){
    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    
    if(GI){
        GI->CreateGame(SessionName);
    }
}

FName UMainWidget::GetConnectionMode(){
    APlayerController* OwningPlayer = GetOwningPlayer<APlayerController>();
    if(OwningPlayer->HasAuthority()){
        return TEXT("Server");
    }
    return TEXT("Client");
}

FName UMainWidget::GetSessionName(){
    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    
    if(GI){
        return GI->GetSessionName();
    }

    return TEXT("Not Found SessionName");
}