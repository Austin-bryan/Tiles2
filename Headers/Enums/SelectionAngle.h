#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
    enum class ESelectionAngle : uint8 { Flat, Uphill, Downhill };
UENUM(BlueprintType)
    enum class ESelectionShape : uint8 { Square, Circle, Triangle };