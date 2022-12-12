#include "BandagedModule.h"
#include "Logger.h"
#include "ModTile.h"
#include "TileSideHandler.h"
#include "Components/TextRenderComponent.h"

// TODO:: Remove module should remove by enum only, only destroying itself if it is unreferenced
//      : if it finds
// TODO:: add ftxt function in logger.obj
int ABandagedModule::bandageCount;
ABandagedModule::ABandagedModule() : ATileModule()
{
    PrimaryActorTick.bCanEverTick = true;
    
    TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
    TextRender->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
    TextRender->SetText(FText::FromString(fstr(bandageCount++)));
    TextRender->SetWorldSize(100);
    TextRender->SetRelativeLocation(FVector(0, 20, 0));
    TextRender->SetRelativeRotation(FRotator(0, 90, 0));
    TextRender->HorizontalAlignment = EHTA_Center;
    TextRender->VerticalAlignment = EVRTA_TextCenter;
}
void ABandagedModule::Init() const                 { Super::Init(); }
void ABandagedModule::AddModTile(AModTile* tile)   { Tiles.AddUnique(tile); }
void ABandagedModule::RemoveModTile(AModTile* tile)
{
    if (!Tiles.Contains(tile))
        return;
    Tiles.Remove(tile);
    tile->SideHandler = tile->oldSideHandler;
    tile->CurrentSide()->RemoveModule(EModule::Bandaged, false);
    tile->SideHandler->PropagateSideLocation(tile->GetActorLocation());
    // tile->CurrentSide()->moduleMap.Remove(EModule::Bandaged);

    if (Tiles.Num() == 0)
        this->Destroy();
}
void ABandagedModule::Tick(const float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    // Log(Tiles.Num(), GetName(), LogParams(FColor::Green, 0));

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

    // for (const auto modTile : Tiles)
        // Log(GetName(), modTile->ID());
    const auto mergeRoot = Tiles[0];
    for (auto* tile : Tiles)
    {
        for (const auto module : tile->Modules())
            mergeRoot->AddModule(module);
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
