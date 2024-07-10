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
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/VerticalBox.h"
#include "ButtonWidget.h"
#include "MyCharacter.h"

void UMainWidget::NativeConstruct() {
    Super::NativeConstruct();

    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (GI) {

        GI->onFoundSession.AddDynamic(this, &UMainWidget::OnFoundSession);
        GI->onFoundSessions.AddDynamic(this, &UMainWidget::OnFoundSessions);
        
        PlayerName->SetText(FText::FromString(GI->GetPlayerName()));
        FString type;
        const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EHatType"), true);
        if (!EnumPtr)
        {
            type = EnumPtr->GetNameStringByValue(static_cast<int64>(GI->GetPlayerHatType()));
            HatOption->SetSelectedOption(type);
        }
        ColorSlider->SetValue(GI->GetPlayerColor().LinearRGBToHSV().R / 360.0f);
    }

    PlayerName->OnTextCommitted.AddDynamic(this, &UMainWidget::SetName);
    HatOption->OnSelectionChanged.AddDynamic(this, &UMainWidget::SetHatType);
    ColorSlider->OnValueChanged.AddDynamic(this, &UMainWidget::SetColor);

    CreateButton->OnClicked.AddDynamic(this, &UMainWidget::CreateGame);
    FindButton->OnClicked.AddDynamic(this, &UMainWidget::FindSessions);
    ExitButton->OnClicked.AddDynamic(this, &UMainWidget::Exit);

}

void UMainWidget::CreateGame(){
    if (RoomName->GetText().IsEmpty() || PlayerName->GetText().IsEmpty()) return;

    CreateButton->SetIsEnabled(false);

    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    
    if(GI){
        GI->CreateGame(RoomName->GetText().ToString());
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

void UMainWidget::SetName(const FText& Text, ETextCommit::Type CommitMethod) {
    if (Text.IsEmpty()) return;

    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (GI) {
        GI->SetPlayerName(Text.ToString());
    }
}

EHatType UMainWidget::StringToEHatType(const FString& String)
{
    if (String == "NoHat") return EHatType::NoHat;
    if (String == "Apple") return EHatType::Apple;
    if (String == "Banana") return EHatType::Banana;
    if (String == "Carrot") return EHatType::Carrot;

    return EHatType::NoHat;
}

void UMainWidget::SetHatType(FString SelectedItem, ESelectInfo::Type SelectionType) {
    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    EHatType type = StringToEHatType(SelectedItem);
    if (GI) {
        GI->SetPlayerHatType(type);
    }

    APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!pc) return;
    AMyCharacter* character = Cast<AMyCharacter>(pc->GetPawn());

    character->SetPlayerHat(type);
}

void UMainWidget::SetColor(float value) {
    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    // HSV to LinearColor
    FLinearColor color = FLinearColor::MakeFromHSV8(static_cast<uint8>(value*360.0f), static_cast<uint8>(255), static_cast<uint8>(255));
    if (GI) {
        GI->SetPlayerColor(color);
    }

    APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!pc) return;
    AMyCharacter* character = Cast<AMyCharacter>(pc->GetPawn());

    character->SetPlayerColor(color);
}
