// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "PlayerData.h"
#include "MyPlayerController.generated.h"


UCLASS()
class MULTIPLAYERGAME_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;

	void OnPossess(APawn* InPawn) override;

	UFUNCTION(Client, Reliable)
	void RequestClientToSendPlayerData();

	UFUNCTION(Server, Reliable)
	void ServerReceivePlayerData(const FPlayerData& ReceivedPlayerData);

	UFUNCTION(Server, Reliable)
	void SendChat(const FText& Text);

	UFUNCTION(Client, Reliable)
	void ReceiveChat(const FText& Text);

	UPROPERTY()
	FPlayerData PlayerData;

	UFUNCTION(Client, Reliable)
	void UpdateAllPlayerData(const TArray<FPlayerData>& AllPlayerData);



protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY()
	UUserWidget* MainWidget;

	void SetWidget();

};
