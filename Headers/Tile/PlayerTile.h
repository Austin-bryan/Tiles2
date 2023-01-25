#pragma once
#include "CoreMinimal.h"
#include "ModTile.h"
#include "PlayerTile.generated.h"

/**
 * These are the tiles that the player interacts with and swipes.
 */
UCLASS()
class TILES2_API APlayerTile : public AModTile
{
	GENERATED_BODY()
public:
	APlayerTile();
};
