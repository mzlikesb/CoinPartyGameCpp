// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UButtonWidget::NativeConstruct() {
	Super::NativeConstruct();

	Button->OnClicked.AddDynamic(this, &UButtonWidget::Click);
}

void UButtonWidget::Click() {
	Button->bIsEnabled = false;
	onClick.Broadcast(Index);
}

void UButtonWidget::Init(FText label, int index) {
	ButtonText->SetText(label);
	Index = index;
}