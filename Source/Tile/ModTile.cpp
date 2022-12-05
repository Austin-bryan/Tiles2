#include "ModTile.h"

#include "Logger.h"
#include "TileSideHandler.h"

AModTile::AModTile()
{
    SideHandler = CreateDefaultSubobject<UTileSideHandler>(TEXT("Side Handler"));
    SideHandler->SetModTile(this);
    SideHandler->AddSide(ETileColor::Blue);
}
void AModTile::SetColor(const ETileColor color, const bool colorSiblings) { Super::SetColor(color, colorSiblings); }

ETileColor AModTile::GetColor()                const { return Super::GetColor(); }
UTileSide* AModTile::CurrentSide()             const { return SideHandler->CurrentSide(); }
TArray<UTileModule*> AModTile::Modules()       const { return SideHandler->CurrentModules(); }
bool AModTile::HasModule(const EModule module) const { return SideHandler->HasModule(module); }
void AModTile::AddModule(UTileModule* module)  const
{
    // NullCheck(CurrentSide());
    // CurrentSide()->AddModule(module);
}
