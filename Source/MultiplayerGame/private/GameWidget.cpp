// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidget.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"



FName UGameWidget::GetConnectionMode() {
    APlayerController* OwningPlayer = GetOwningPlayer<APlayerController>();
    if (OwningPlayer->HasAuthority()) {
        return TEXT("Server");
    }
    return TEXT("Client");
}

FName UGameWidget::GetSessionName() {
    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (GI) {
        return GI->GetSessionName();
    }

    return TEXT("Not Found SessionName");
}