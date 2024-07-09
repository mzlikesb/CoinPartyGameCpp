// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EHatType.h"
#include "PlayerData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHatType HatType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color;

	FPlayerData() : Name("Player"), HatType(EHatType::NoHat), Color(FLinearColor::White) {}
};
