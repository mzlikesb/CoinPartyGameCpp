// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "PlayerData.h"
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
	
	UFUNCTION(Client, Reliable)
	void RequestClientToSendPlayerData();

	UFUNCTION(Server, Reliable)
	void ServerReceivePlayerData(const FPlayerData& playerData);

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY()
	FPlayerData PlayerData;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UUserWidget> WidgetClass;

	void SetWidget();

};
