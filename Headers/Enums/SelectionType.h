#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESelectionType : uint8
{
	Flat, Uphill, Downhill, Square, Circle, Triangle
};