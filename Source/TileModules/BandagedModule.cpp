#include "BandagedModule.h"
#include "Logger.h"
#include "ModTile.h"
#include "TileSide.h"
#include "TileSideHandler.h"
#include "Components/TextRenderComponent.h"

int ABandagedModule::bandageCount;
ABandagedModule::ABandagedModule() : ATileModule()
{
    PrimaryActorTick.bCanEverTick = true;
#ifdef SHOW_BANDAGE_ID
    bandageID = bandageCount;
    TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
    TextRender->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
    TextRender->SetText(ftxt(bandageCount++));
    TextRender->SetWorldSize(30);
    TextRender->SetRelativeLocation(FVector(0, 20, 0));
    TextRender->SetRelativeRotation(FRotator(0, 90, 0));
    TextRender->HorizontalAlignment = EHTA_Center;
    TextRender->VerticalAlignment   = EVRTA_TextCenter;
#endif
}
void ABandagedModule::Init() const { Super::Init(); }
void ABandagedModule::AddModTile(AModTile* tile) { Tiles.AddUnique(tile); }
void ABandagedModule::RemoveModTile(AModTile* tile)
{
    if (!Tiles.Contains(tile))
        return;
    Tiles.Remove(tile);

    tile->ResetSideHandler();
    tile->CurrentSide()->RemoveModule(EModule::Bandaged, false);

    if (Tiles.Num() == 0)
        this->Destroy();
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
        if (tile != mergeRoot)
        for (const auto module : tile->Modules())
        {
            if (mergeRoot->HasModule(module->ModuleType()))
                 tile->CurrentSide()->RemoveModule(module);
            else mergeRoot->AddModule(module);
        }
        tile->SetSideHandler(mergeRoot->GetSideHandler());
    }
}
void ABandagedModule::SetColor(const ETileColor color, const bool propagate)
{
    Super::SetColor(color, propagate);
    for (const auto tile : Tiles)
        tile->SetColor(color, false);
}