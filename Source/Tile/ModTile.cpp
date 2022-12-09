#include "ModTile.h"
#include "TileSideHandler.h"

AModTile::AModTile()
{
    SideHandler = CreateDefaultSubobject<UTileSideHandler>(TEXT("Side Handler"));
    SideHandler->SetModTile(this);
}

void AModTile::SetColor(const ETileColor color, const bool propagate)
{
    Super::SetColor(color, propagate);

    if (propagate)
    for (const auto module : CurrentSide()->Modules())
        module->SetColor(color, true);
}
void AModTile::BeginPlay()
{
    Super::BeginPlay();
    SideHandler->AddSide(ETileColor::Blue);
}
ETileColor AModTile::GetColor()                    const { return Super::GetColor(); }
ATileSide* AModTile::CurrentSide()                 const { return SideHandler->CurrentSide(); }
TArray<ATileModule*> AModTile::Modules()           const { return SideHandler->CurrentModules(); }
bool AModTile::HasModule(const EModule module)     const { return SideHandler->HasModule(module); }
const UTileSideHandler* AModTile::GetSideHandler() const { return SideHandler; }
void AModTile::AddModule(ATileModule* module, const bool addToSiblings)  const { CurrentSide()->AddModule(module); }
