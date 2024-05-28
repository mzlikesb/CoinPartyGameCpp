// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERGAME_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UUserWidget> WidgetClass;

	void SetWidget();

};
