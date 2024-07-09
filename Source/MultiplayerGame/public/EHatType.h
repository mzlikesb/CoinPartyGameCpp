#pragma once

#include "EHatType.generated.h"

UENUM(BlueprintType)
enum class EHatType : uint8 {
	NoHat UMETA(DisplayName = "NoHat"),
	Apple UMETA(DisplayName = "Apple"),
	Banana UMETA(DisplayName = "Banana"),
	Carrot UMETA(DisplayName = "Carrot"),
};

