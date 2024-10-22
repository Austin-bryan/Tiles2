#pragma once
#include "CoreMinimal.h"
#include "Tile.h"
#include "TileModule.h"
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

    ETileColor GetColor() const override;
    void BeginPlay() override;
    ATileSide* CurrentSide() const;
    TArray<ATileModule*> Modules() const;
    
    bool HasModule(const EModule module) const;
    ATileModule* GetModule(const EModule module) const;
    void SetColor(
        const ETileColor color,
        bool propagate)
    override;
    
    int SideCount() const;
    void AddModule(ATileModule* module) const;
    void SetSideHandler(UTileSideHandler* newSideHandler);
    void ResetSideHandler();
    UTileSideHandler* GetSideHandler() const;
    TArray<ATileSide*> GetSides() const;
protected:
    UPROPERTY(VisibleAnywhere)
    UTileSideHandler* SideHandler;
private:
    UPROPERTY()
    UTileSideHandler* oldSideHandler; 
};
