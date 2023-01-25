#include "ModTile.h"

#include "TileSide.h"
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

int  AModTile::SideCount()                       const { return SideHandler->SideCount(); }
bool AModTile::HasModule(EModule module)         const { return SideHandler->HasModule(module); }
void AModTile::AddModule(ATileModule* module)    const { CurrentSide()->AddModule(module); }
ETileColor AModTile::GetColor()                  const { return Super::GetColor(); }
ATileSide* AModTile::CurrentSide()               const { return SideHandler->CurrentSide(); }
TArray<ATileModule*> AModTile::Modules()         const { return SideHandler->CurrentModules(); }
UTileSideHandler* AModTile::GetSideHandler()     const { return SideHandler; }
TArray<ATileSide*> AModTile::GetSides()          const { return SideHandler->GetSides(); }
ATileModule* AModTile::GetModule(EModule module) const { return SideHandler->GetModule(module); } 
void AModTile::SetSideHandler(UTileSideHandler* newSideHandler) { oldSideHandler = SideHandler, SideHandler = newSideHandler; }
void AModTile::ResetSideHandler()
{
    SideHandler = oldSideHandler;
    SideHandler->PropagateSideLocation(GetActorLocation());
}