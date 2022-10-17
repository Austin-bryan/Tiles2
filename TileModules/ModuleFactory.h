#pragma once
#include "CoreMinimal.h"

enum class EModule;
struct FParameter;
class ATile;
class UTileModule;

/**
 * 
 */
class TILES2_API ModuleFactory
{
public:
    static UTileModule* Produce(const EModule module, ATile* modTile, const TArray<FParameter>* const parameters = nullptr);
};
