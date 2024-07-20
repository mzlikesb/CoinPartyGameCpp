#pragma once

#include "CoreMinimal.h"
#include "PlayerScore.generated.h"

USTRUCT(BlueprintType)
struct FPlayerScore
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint8 PlayerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint8 Coins;

    FPlayerScore() : PlayerID(0), Name("Player"), Coins(0) {}
};