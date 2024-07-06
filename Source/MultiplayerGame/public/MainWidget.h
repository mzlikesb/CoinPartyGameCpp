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
	class UEditableText* PlayerName;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UVerticalBox* SessionList;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UButtonWidget> ButtonWidget;

	UFUNCTION(BlueprintCallable)
	void CreateGame();

	UFUNCTION(BlueprintCallable)
	void FindSessions();

	UFUNCTION(BlueprintCallable)
	void JoinSessions(int32 SessionIndex);

	UFUNCTION(BlueprintCallable)
	void Exit();

	UFUNCTION()
	void OnFoundSession(FString SessionName, int32 MaxPlayers, int32 CurrentPlayers, int32 SessionIndex);
	UFUNCTION()
	void OnFoundSessions();

protected:

	virtual void NativeConstruct() override;




};