// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerScore.h"
#include "GameOverWidget.generated.h"

class UScoreWidget;

UCLASS()
class MULTIPLAYERGAME_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UButton* RestartButton;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UButton* ExitButton;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UVerticalBox* ScoreList;

	UFUNCTION()
	void ShowScore(TArray<FPlayerScore> AllPlayerScore);

	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void ExitGame(); 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UScoreWidget> ScoreWidgetClass;

protected:
	void NativeConstruct() override;
};
