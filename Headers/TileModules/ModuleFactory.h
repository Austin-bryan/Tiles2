#pragma once
#include "CoreMinimal.h"

enum class EModule : uint8;
struct FParameter;
class AModTile;
class ATileModule;

class TILES2_API ModuleFactory
{
public:
    static ATileModule* Produce(const EModule module, AModTile* modTile, const TArray<FParameter>& parameters = TArray<FParameter>());
};
