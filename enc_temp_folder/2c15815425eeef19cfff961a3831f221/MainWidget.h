// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERGAME_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UButton* CreateButton;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UButton* FindButton;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UButton* ExitButton;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UEditableText* GameName;

	void CreateGame();

	void FindSessions();

	void JoinSessions(int32 SessionIndex);


protected:

	virtual void NativeConstruct() override;

	void OnFoundSessions(FString SessionName, int32 MaxPlayers, int32 CurrentPlayers, int32 SessionIndex);

	void OnClickExitButton();


};