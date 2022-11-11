#pragma once
#include "CoreMinimal.h"
#include "Tile.h"
#include "PlayerTile.generated.h"

/**
 * These are the tiles that the player interacts with and swipes.
 */
UCLASS()
class TILES2_API APlayerTile : public ATile
{
	GENERATED_BODY()
public:
	APlayerTile();
};
