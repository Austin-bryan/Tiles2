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
    //TODO:: remove this friend class
    friend class ABandagedModule;
public:
    AModTile();

    ETileColor GetColor() const override;
    void BeginPlay() override;
    ATileSide* CurrentSide() const;
    TArray<ATileModule*> Modules() const;
    
    bool HasModule(const EModule module) const;
    void SetColor(const ETileColor color,
        bool propagate) override;
    void AddModule(ATileModule* module,
        bool addToSiblings = true) const;
    const UTileSideHandler* GetSideHandler() const;

    TSharedPtr<TArray<AModTile*>> siblings;
protected:
    UPROPERTY(VisibleAnywhere)
    UTileSideHandler* SideHandler;
private:
    UPROPERTY()
    UTileSideHandler* oldSideHandler; 
};
