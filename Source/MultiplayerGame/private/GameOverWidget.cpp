// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Components/Button.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/VerticalBox.h"
#include "ScoreWidget.h"

void UGameOverWidget::NativeConstruct() {
    Super::NativeConstruct();

    RestartButton->OnClicked.AddDynamic(this, &UGameOverWidget::RestartGame);
    ExitButton->OnClicked.AddDynamic(this, &UGameOverWidget::ExitGame);

    APlayerController* OwningPlayer = GetOwningPlayer<APlayerController>();
    if (OwningPlayer->HasAuthority()) {
        RestartButton->SetIsEnabled(true);
    }
    else {
        RestartButton->SetIsEnabled(false);
    }
}

void UGameOverWidget::ShowScore(TArray<FPlayerScore> AllPlayerScore) {
    ScoreList->ClearChildren();
    for (FPlayerScore Score : AllPlayerScore) {
        UScoreWidget* widget = CreateWidget<UScoreWidget>(GetWorld(), ScoreWidgetClass);
        widget->SetScore(Score);
        ScoreList->AddChild(widget);
    }
}

void UGameOverWidget::RestartGame() {
    RestartButton->SetIsEnabled(false);
    GetWorld()->ServerTravel("Game?Listen", true);
}

void UGameOverWidget::ExitGame() {
    UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (GI) GI->EndGame();
}