// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidget.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UGameWidget::NativeConstruct() {
    Super::NativeConstruct();

    RoomName->SetText(GetRoomName());
    ConnectionMode->SetText(GetConnectionMode());
    ExitButton->OnClicked.AddDynamic(this, &UGameWidget::ExitGame);
}

FText UGameWidget::GetConnectionMode() {
    APlayerController* OwningPlayer = GetOwningPlayer<APlayerController>();
    if (OwningPlayer->HasAuthority()) {
        return FText::FromString(TEXT("Server"));
    }
    return FText::FromString(TEXT("Client"));
}

FText UGameWidget::GetRoomName() {
    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (GI) {
        return GI->GetRoomName();
    }

    return  FText::FromString(TEXT("Not Found SessionName"));
}

void UGameWidget::ExitGame() {
    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (GI) GI->EndGame();
}