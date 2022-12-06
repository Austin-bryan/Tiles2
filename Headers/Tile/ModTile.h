#pragma once
#include "CoreMinimal.h"
#include "Tile.h"
#include "TileModule.h"
#include "TileSide.h"
#include "ModTile.generated.h"

class UTileSideHandler;
class ATileSide;

/**
 * Base class for Tiles that are able to have Modules, as opposed to tiles like TargetTile that are just a color.
 */
UCLASS()
class TILES2_API AModTile : public ATile
{
	GENERATED_BODY()
public:
    AModTile();

    void SetColor(
        const ETileColor color,
        bool colorSiblings = true) override;
    ETileColor GetColor() const override;

    void BeginPlay() override;

    ATileSide* CurrentSide() const;
    TArray<ATileModule*> Modules() const;
    
    bool HasModule(const EModule module) const;
    void AddModule(ATileModule* module, bool addToSiblings = true) const;
    void CenterSprites() const;
protected:
    UPROPERTY(VisibleAnywhere)
    UTileSideHandler* SideHandler;
};
