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


	FText GetConnectionMode();

	FText GetRoomName();

	UFUNCTION()
	void ExitGame();

protected:
	virtual void NativeConstruct() override;

};
