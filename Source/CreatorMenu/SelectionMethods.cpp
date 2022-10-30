#include "SelectionMethods.h"
#include "Coord.h"
#include "CreatorTile.h"

TArray<ACreatorTile*> LocalWandSelection::MagicWandSelectedTiles;

void IndividualSelection::Select()
{
    Reset();
    SelectionMethod::Select();
}
void ControlSelection::Select()
{
    if  (creatorTile->IsSelected())
         creatorTile->Deselect();
    else creatorTile->Select();
}
void AltSelection::MultiSelect()
{
    FCoordPtr firstCoord, lastCoord;
    if (!GetFirstAndLastCoord(true, firstCoord, lastCoord))
        return;
    // LinePlotter.GetPointsOnLine(firstCoord.X, firstCoord.Y, lastCoord.X, lastCoord.Y).Where(c => c.GetCreatorTile().MatchesFirstTile()).ToList().ForEach(c => c.GetCreatorTile().Select());
}

LocalWandSelection::LocalWandSelection(ACreatorTile* creatorTile, bool isFirst) : SelectionMethod(creatorTile)
{
    // if (isFirst && !Input.GetKey(KeyCode.LeftShift))
    {
        MagicWandSelectedTiles.Empty();
        DeselectAll(false);
    }
}
void LocalWandSelection::OnDeselect() { MagicWandSelectedTiles.Empty(); }
void LocalWandSelection::MultiSelect()
{
    auto neighbors = TArray<ACreatorTile*>();

    auto addNeighbor = [neighbors](int x, int y)
    {
        // neighbors.Add((ACreatorTile*)CreatorBoardInst[creatorTile.Coord + new Coord(x, y)]); }
    };
    addNeighbor( 1, 0);
    addNeighbor( 0,-1);
    addNeighbor( 0, 1);
    addNeighbor(-1, 0);

    for (const auto& neighbor : neighbors)
    {
        if (neighbor == nullptr || MagicWandSelectedTiles.Contains(neighbor))
            continue;
        if (GlobalWandSelection::ShouldSelect(neighbor, creatorTile))
        {
            MagicWandSelectedTiles.Add(neighbor);
            // neighbor->selectionMethod = new LocalWandSelection(neighbor, false);
            // neighbor->selectionMethod.Select(); 
        }
    }
}

void GlobalWandSelection::MultiSelect()
{
    // for (const auto& [k, c] : CreatorBoardInst.Tiles)
    {
        // var creatorTile = (CreatorTile)tile;
        // if (ShouldSelect(c, this->creatorTile))
        //     c->Select();
    }
}
bool GlobalWandSelection::ShouldSelect(ACreatorTile* creatorTileA, ACreatorTile* creatorTileB)
{
    // return creatorTileA->IsSelected && creatorTileAColor == creatorTileB.Color && !(creatorTileA.SideHandler.HasGap() || creatorTileA.SideHandler.HasWrap())
    // || creatorTileA.SideHandler.HasGap() && creatorTileB.SideHandler.HasGap() || creatorTileA.SideHandler.HasWrap() && creatorTileB.SideHandler.HasWrap();
    return false;
}

void ShiftSelection::MultiSelect()
{
    FCoordPtr firstCoord, lastCoord;
    if (!GetFirstAndLastCoord(false, firstCoord, lastCoord))
        return;

    auto GetIndex = [](const bool useLargest, const float firstIndex, const float lastIndex)
    {
        return useLargest
            ? firstIndex > lastIndex
                ? firstIndex
                : lastIndex
            : firstIndex < lastIndex
                ? firstIndex
                : lastIndex;
    };

    const FCoordPtr minCoord = MakeShared<FCoord>(
        GetIndex(false, firstCoord->X(), lastCoord->X()),
        GetIndex(false, firstCoord->Y(), lastCoord->Y()));
    const FCoordPtr maxCoord = MakeShared<FCoord>(
        GetIndex(true, firstCoord->X(), lastCoord->X()),
        GetIndex(true, firstCoord->Y(), lastCoord->Y()));
    DeselectAll(true);                                    

    for (auto i = minCoord->X(); i <= maxCoord->X(); i += 0.5f)
        for (auto j = minCoord->Y(); j <= maxCoord->Y(); j += 0.5f)
        {
            // if (CreatorBoardInst.Tiles.ContainsKey((i, j)) && !CreatorBoardInst.Tiles[(i, j)].RefEquals(FirstTile))
            // {
            //     var creatorTile = (CreatorBoardInst.Tiles[(i, j)] as CreatorTile);
            //     if (creatorTile == null || !creatorTile.MatchesFirstTile())
            //         continue;
            //     creatorTile.Select();
            // }
        }
}
