#include "BandagedModule.h"
#include "Logger.h"
#include "ModTile.h"
#include "TileSideHandler.h"

ABandagedModule::ABandagedModule() : ATileModule() { PrimaryActorTick.bCanEverTick = true; }
void ABandagedModule::Init() const                 { Super::Init(); }
void ABandagedModule::AddModTile(AModTile* tile)   { Tiles.AddUnique(tile); }
void ABandagedModule::RemoveModTile(AModTile* tile)
{
    if (!Tiles.Contains(tile))
        return;
    Tiles.Remove(tile);
    tile->SideHandler = tile->oldSideHandler;
}
void ABandagedModule::Tick(const float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    FString s;
    for (const auto t : Tiles)
        s += t->ID() + ", "_f;
    TileIDs = s;
}
void ABandagedModule::Finish()
{
    // Center Sprites
    FVector sum;
    for (const AModTile* tile : Tiles)
        sum += tile->GetSideHandler()->CurrentSide()->GetActorLocation();
    ModTile->GetSideHandler()->PropagateSideLocation(sum / Tiles.Num());

    const auto mergeRoot = Tiles[0];

    for (auto* tile : Tiles)
    {
        for (const auto module : tile->Modules())
        {
            if (tile == mergeRoot)
                continue;
            if (!mergeRoot->HasModule(module->ModuleType()))
                 mergeRoot->AddModule(module);
            else module->Destroy();
        }
        tile->oldSideHandler = tile->SideHandler;
        tile->SideHandler = mergeRoot->SideHandler;
    }
}
void ABandagedModule::SetColor(const ETileColor color, const bool propagate)
{
    Super::SetColor(color, propagate);
    for (const auto tile : Tiles) 
        tile->SetColor(color, false);
}
