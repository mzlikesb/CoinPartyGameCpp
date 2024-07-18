// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidget.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "MyPlayerController.h"
#include "Components/VerticalBox.h"

void UGameWidget::NativeConstruct() {
    Super::NativeConstruct();

    RoomName->SetText(GetRoomName());
    ConnectionMode->SetText(GetConnectionMode());
    ExitButton->OnClicked.AddDynamic(this, &UGameWidget::ExitGame);

    ChatText->OnTextCommitted.AddDynamic(this, &UGameWidget::SendChat);
    AMyPlayerController* PC = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
    if (PC) {
        UpdatePlayerName(FText::FromString(PC->PlayerData.Name));
    }
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

void UGameWidget::SendChat(const FText& Text, ETextCommit::Type CommitMethod) {
    if (ChatText->GetText().IsEmpty() || CommitMethod != ETextCommit::OnEnter) return;

    AMyPlayerController* PC = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
    if (PC) {
        PC->SendChat(Text);
    }
    ChatText->SetText(FText::GetEmpty());
}

void UGameWidget::ReceiveChat(const FText& Text) {
    UTextBlock* Chat = NewObject<UTextBlock>(this);
    Chat->SetText(Text);
    ChatList->AddChild(Chat);
}

void UGameWidget::UpdatePlayerName(const FText& Name) {
    PlayerName->SetText(Name);
}

void UGameWidget::UpdateAllPlayerData(TArray<FPlayerData> PlayersData) {
    PlayerList->ClearChildren();
    for (FPlayerData data : PlayersData)
    {
        UTextBlock* Text = NewObject<UTextBlock>(this);
        Text->SetText(FText::FromString(data.Name));
        PlayerList->AddChild(Text);
    }
}

