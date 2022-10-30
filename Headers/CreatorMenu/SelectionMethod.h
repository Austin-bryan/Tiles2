#pragma once
#include "CoreMinimal.h"
#include "ForwardDeclares.h"

class ACreatorTile;

class SelectionMethod
{
public:
    static ACreatorTile* FirstTile() { return firstSelectedTiles.Num() > 0 ? firstSelectedTiles[0] : nullptr; }
    static TArray<ACreatorTile*>& SelectedTiles(){ return selectedTiles; }
    static void DeselectAll(bool preserveFirstTiles);
    static void SetFirstTiles(ACreatorTile* creatorTile);

    explicit SelectionMethod(ACreatorTile* _creatorTile) { creatorTile = _creatorTile; }
    virtual ~SelectionMethod(){ }

    virtual void MultiSelect() { }
    virtual void Select();
protected:
    static TArray<ACreatorTile*> selectedTiles;
    ACreatorTile* creatorTile;
    void Reset();
    bool GetFirstAndLastCoord(bool chainSelections, FCoordPtr& firstCoord, FCoordPtr& lastCoord);
private:
    static ACreatorTile* RecentTile;
    static TArray<ACreatorTile*> firstSelectedTiles;
};
