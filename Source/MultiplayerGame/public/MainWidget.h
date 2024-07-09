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
	class UEditableText* RoomName;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UVerticalBox* SessionList;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UButtonWidget> ButtonWidget;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UEditableText* PlayerName;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UComboBoxString* HatOption;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class USlider* ColorSlider;

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

	UFUNCTION()
	void SetName(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void SetHatType(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void SetColor(float value);

	UFUNCTION()
	EHatType StringToEHatType(const FString& String);

protected:

	virtual void NativeConstruct() override;


};