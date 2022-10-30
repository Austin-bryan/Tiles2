#pragma once
#include "CoreMinimal.h"
#include "SelectionMethod.h"

class ACreatorTile;

class IndividualSelection : public SelectionMethod
{
public:
    explicit IndividualSelection(ACreatorTile* creatorTile) : SelectionMethod(creatorTile) { }
    void Select() override;
};

class ControlSelection : public SelectionMethod
{
public:
    explicit ControlSelection(ACreatorTile* creatorTile) : SelectionMethod(creatorTile) { }
    void Select() override;
};

class AltSelection : public SelectionMethod
{
public:
    explicit AltSelection(ACreatorTile* creatorTile) : SelectionMethod(creatorTile) { }
    void MultiSelect() override;
};

class LocalWandSelection : public SelectionMethod
{
public:
    LocalWandSelection(ACreatorTile* creatorTile, bool isFirst);

    static void OnDeselect();
    void MultiSelect() override;
protected:
    static TArray<ACreatorTile*> MagicWandSelectedTiles;
};

class GlobalWandSelection : public SelectionMethod
{
public:
    explicit GlobalWandSelection(ACreatorTile* creatorTile) : SelectionMethod(creatorTile) { }
    void MultiSelect() override;
    static bool ShouldSelect(ACreatorTile* creatorTileA, ACreatorTile* creatorTileB);
};

class ShiftSelection : public SelectionMethod
{
public:
    explicit ShiftSelection(ACreatorTile* creatorTile) : SelectionMethod(creatorTile) { }
    void MultiSelect() override;
};