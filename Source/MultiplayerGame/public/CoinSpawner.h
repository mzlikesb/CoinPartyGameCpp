// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoinSpawner.generated.h"

class UBoxComponent;
class ACoin;

UCLASS()
class MULTIPLAYERGAME_API ACoinSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ACoinSpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 NumberOfCoins = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACoin> CoinClass;
};