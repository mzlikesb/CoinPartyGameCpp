// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidget.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UGameWidget::NativeConstruct() {
    Super::NativeConstruct();

    RoomName->SetText(GetSessionName());
    ConnectionMode->SetText(GetConnectionMode());
}

FText UGameWidget::GetConnectionMode() {
    APlayerController* OwningPlayer = GetOwningPlayer<APlayerController>();
    if (OwningPlayer->HasAuthority()) {
        return FText::FromString(TEXT("Server"));
    }
    return FText::FromString(TEXT("Client"));
}

FText UGameWidget::GetSessionName() {
    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (GI) {
        return GI->GetSessionName();
    }

    return  FText::FromString(TEXT("Not Found SessionName"));
}