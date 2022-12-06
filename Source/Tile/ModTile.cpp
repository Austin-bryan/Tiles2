#include "ModTile.h"

#include "Logger.h"
#include "TileSideHandler.h"
#include "TileSideHandler.h"

AModTile::AModTile()
{
    SideHandler = CreateDefaultSubobject<UTileSideHandler>(TEXT("Side Handler"));
    SideHandler->SetModTile(this);
}
void AModTile::SetColor(const ETileColor color, const bool colorSiblings) { Super::SetColor(color, colorSiblings); }

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
    if (siblings.IsValid())
    for (ATile* sibling : *siblings)
    if (Cast<AModTile>(sibling)->SideHandler->HasModule(module))
        return true;
    return SideHandler->HasModule(module);
}

void AModTile::AddModule(ATileModule* module, const bool addToSiblings)  const
{
    CurrentSide()->AddModule(module);
    
    // if (addToSiblings)
    // for (ATile* sibling : *siblings)
    //     Cast<AModTile>(sibling)->AddModule(module, false);
}
void AModTile::OnMerge() const
{
    if (!siblings.IsValid())
        return;
    FVector sum;

    // Center Sprites
    for (ATile* sibling : *siblings)
        sum += Cast<AModTile>(sibling)->SideHandler->CurrentSide()->GetActorLocation();
    SideHandler->PropagateSideLocation(sum / siblings->Num());

    if ((*siblings)[0] != this)
    {
        // TODO:: Add modules from all tiles at least once
        // TODO:: Remove any duplicate modules
    }
}