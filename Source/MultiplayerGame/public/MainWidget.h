// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERGAME_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	int a;
	
	UFUNCTION(BlueprintCallable)
	void CreateGame(FName SessionName);

protected:
	UFUNCTION(BlueprintCallable)
	FName GetConnectionMode();

	UFUNCTION(BlueprintCallable)
	FName GetSessionName();
};
