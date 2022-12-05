#include "TileSideHandler.h"

#include "ModTile.h"
#include "TileModule.h"
#include "TileSide.h"

int  UTileSideHandler::SideCount()    const { return GetNumChildrenComponents();}
int  UTileSideHandler::TileID()       const { return ModTile()->ID();}
int  UTileSideHandler::CurrentIndex() const { return currentIndex;}
bool UTileSideHandler::IsMultiSided() const { return SideCount() > 1;}
void UTileSideHandler::BeginPlay()                             { Super::BeginPlay(); }
void UTileSideHandler::SetModTile(AModTile* _modTile)          { modTile = _modTile; }
void UTileSideHandler::SetColor(const ETileColor _color) const { CurrentSide()->SetColor(_color); }

UTileSide* UTileSideHandler::CurrentSide()              const { return (*this)[currentIndex];}
ETileColor UTileSideHandler::Color()                    const { return CurrentSide()->ModTile()->GetColor();}
AModTile*  UTileSideHandler::ModTile()                  const { return modTile;}
TArray<UTileModule*> UTileSideHandler::CurrentModules() const { return CurrentSide()->Modules();}

void UTileSideHandler::InitModules() const
{
    for (const auto side : GetSides())
        for (const auto module : side->Modules())
            module->Init();
}
TArray<UTileSide*> UTileSideHandler::GetSides() const
{
    TArray<USceneComponent*> children;
    GetChildrenComponents(false, children);

    TArray<UTileSide*> sides;
    for (const auto child : children)
        if (auto* side = dynamic_cast<UTileSide*>(child))
            sides.Add(side);
    return GetSides();
}
UTileSide* UTileSideHandler::operator[](const int index) const { return GetSides()[index]; }

void UTileSideHandler::RemoveAll()
{
    const int sideCount = SideCount();
    for (int i = 0; i < sideCount; i++)
        RemoveSide(i);
    currentIndex = 0;
}
void UTileSideHandler::RemoveSide(int index)
{
    if (SideCount() == 1)
        return;
    index = index == -1 ? CurrentIndex() : index;
    (*this)[index]->DestroyComponent();
    currentIndex--;
}
void UTileSideHandler::AddSide(AModTile* modTile, ETileColor color)  { currentIndex++; }
bool UTileSideHandler::HasModule(const EModule module)         const { return CurrentSide()->HasModule(module); }
UTileModule* UTileSideHandler::GetModule(const EModule module) const { return CurrentSide()->GetModule(module);}

void UTileSideHandler::SetToDefault() { ChangeSide(0); }
void UTileSideHandler::NextSide()     { ChangeSide(currentIndex + 1);}
void UTileSideHandler::PrevSide()     { ChangeSide(currentIndex - 1); }
void UTileSideHandler::ChangeSide(const int newSide, const bool shouldHide)
{
    if (shouldHide)
        (*this)[currentIndex]->SetVisibility(false);
    currentIndex = newSide % SideCount() - 1;
    
    (*this)[currentIndex]->SetVisibility(true);
    // TODO:: if playerboard, call on side reveal on all side modules
}