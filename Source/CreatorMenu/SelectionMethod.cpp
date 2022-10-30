#include "SelectionMethod.h"
#include "CreatorTile.h"

ACreatorTile* SelectionMethod::RecentTile;
TArray<ACreatorTile*> SelectionMethod::firstSelectedTiles;
TArray<ACreatorTile*> SelectionMethod::selectedTiles;


void SelectionMethod::DeselectAll(const bool preserveFirstTiles)
{
    auto tilesToDeselect = selectedTiles.FilterByPredicate([preserveFirstTiles](ACreatorTile* c){
        return !(preserveFirstTiles && firstSelectedTiles.Contains(c)); });
    for (const auto& creatorTile : tilesToDeselect)
        creatorTile->Deselect();
}
void SelectionMethod::SetFirstTiles(ACreatorTile* creatorTile)
{
    firstSelectedTiles.Empty();
    // if (SymmetryButton.SymmetryMethod != null)
        // FirstSelectedTiles = SymmetryButton.SymmetryMethod.GetTiles(creatorTile.Coord).Where(c => c != null).ToList();
    firstSelectedTiles.Insert(creatorTile, 0);
}
void SelectionMethod::Select() 
{
    creatorTile->Select();
    MultiSelect();
}
void SelectionMethod::Reset()
{
    SetFirstTiles(creatorTile);
    DeselectAll(false);
}
bool SelectionMethod::GetFirstAndLastCoord(const bool chainSelections, FCoordPtr& firstCoord, FCoordPtr& lastCoord)
{   
    if (firstSelectedTiles.Num() < 1 || RecentTile == nullptr)
        return false;
    firstCoord = firstSelectedTiles[0]->GetCoord();
    lastCoord  = RecentTile->GetCoord();

    if (chainSelections)
        firstSelectedTiles[0] = RecentTile;
    return true;
}