// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FonClick, int32, Index);
UCLASS()
class MULTIPLAYERGAME_API UButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UButton* Button;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* ButtonText;

	UPROPERTY(BlueprintAssignable)
	FonClick onClick;

	UFUNCTION()
	void Init(FText label, int index);


protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void Click();


	UPROPERTY()
	int Index;

};
