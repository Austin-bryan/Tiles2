#include "BandagedModule.h"
#include "Logger.h"
#include "ModTile.h"

ABandagedModule::ABandagedModule() : ATileModule()
{
    PrimaryActorTick.bCanEverTick = true;
}
void ABandagedModule::Tick(const float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!Tiles.IsValid())   
        return;
    FString s;

    for (const auto t : *Tiles)
        s += t->ID() + ", "_f;
    TileIDs = s;
}