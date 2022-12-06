#include "ModTile.h"

#include "Logger.h"
#include "TileSideHandler.h"

AModTile::AModTile()
{
    SideHandler = CreateDefaultSubobject<UTileSideHandler>(TEXT("Side Handler"));
    SideHandler->SetModTile(this);
}
void AModTile::SetColor(const ETileColor color, const bool colorSiblings)
{
    Super::SetColor(color, colorSiblings);
    
    if (colorSiblings && siblings)
        for (AModTile* sibling : *siblings)
            sibling->SetColor(color, false);
}

void AModTile::BeginPlay()
{
    Super::BeginPlay();
    SideHandler->AddSide(ETileColor::Blue);
}
ETileColor AModTile::GetColor()          const { return Super::GetColor(); }
ATileSide* AModTile::CurrentSide()       const { return SideHandler->CurrentSide(); }
TArray<ATileModule*> AModTile::Modules() const { return SideHandler->CurrentModules(); }
bool AModTile::HasModule(const EModule module) const
{
    // if (siblings.IsValid())
    // for (const AModTile* sibling : *siblings)
    // if (sibling->SideHandler->HasModule(module))
        // return true;
    return SideHandler->HasModule(module);
}
void AModTile::BandagedWith(const TSharedPtr<TArray<AModTile*>> sharedSiblings)
{
    siblings = sharedSiblings;
    sharedSiblings->AddUnique(this);
}

void AModTile::AddModule(ATileModule* module, const bool addToSiblings)  const
{
    CurrentSide()->AddModule(module);
    
    // if (addToSiblings)
    // for (const AModTile* sibling : *siblings)
    //     sibling->AddModule(module, false);
}
void AModTile::OnMerge()
{
    if (!siblings.IsValid())
        return;
    FVector sum;
    
    // Center Sprites
    for (const AModTile* sibling : *siblings)
        sum += sibling->SideHandler->CurrentSide()->GetActorLocation();
    SideHandler->PropagateSideLocation(sum / siblings->Num());

    const auto mergeRoot = (*siblings)[0];
    if (mergeRoot != this)
    {
        for (const auto module : Modules())
        {
            if (!mergeRoot->HasModule(module->ModuleType()))
                 mergeRoot->AddModule(module);
            else module->Destroy();
        }
        // TODO:: cache the original SideHandler to reuse for when they are split
        SideHandler = mergeRoot->SideHandler;
    }
}