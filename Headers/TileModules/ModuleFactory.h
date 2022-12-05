#pragma once
#include "CoreMinimal.h"

enum class EModule : uint8;
struct FParameter;
class AModTile;
class UTileModule;

class TILES2_API ModuleFactory
{
public:
    static UTileModule* Produce(const EModule module, AModTile* modTile, const TArray<FParameter>& parameters = TArray<FParameter>());
};
