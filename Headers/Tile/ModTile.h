#pragma once
#include "CoreMinimal.h"
#include "Tile.h"
#include "ModTile.generated.h"

/**
 * Base class for Tiles that are able to have Modules, as opposed to tiles like TargetTile that are just a color.
 */
UCLASS()
class TILES2_API AModTile : public ATile
{
	GENERATED_BODY()
};
