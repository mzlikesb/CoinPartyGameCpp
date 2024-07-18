// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERGAME_API UGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* RoomName;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* ConnectionMode;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UButton* ExitButton;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UEditableText* ChatText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UVerticalBox* ChatList;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* PlayerName;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UVerticalBox* PlayerList;

	FText GetConnectionMode();

	FText GetRoomName();

	UFUNCTION()
	void SendChat(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void ReceiveChat(const FText& Text);


	UFUNCTION()
	void ExitGame();

	UFUNCTION()
	void UpdatePlayerName(const FText& Name);

	UFUNCTION()
	void UpdateAllPlayerData(TArray<FPlayerData> PlayersData);

protected:
	virtual void NativeConstruct() override;

};
