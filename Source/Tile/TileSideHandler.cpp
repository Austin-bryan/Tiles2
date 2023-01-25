#include "TileSideHandler.h"
#include "ModTile.h"
#include "TileModule.h"
#include "TileSide.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UTileSideHandler::UTileSideHandler() { }
int  UTileSideHandler::SideCount()    const { return GetOwner()->Children.Num(); }
int  UTileSideHandler::TileID()       const { return ModTile()->ID(); }
int  UTileSideHandler::CurrentIndex() const { return currentIndex; }
bool UTileSideHandler::IsMultiSided() const { return SideCount() > 1; }
void UTileSideHandler::BeginPlay()          { Super::BeginPlay(); }

void UTileSideHandler::SetModTile(AModTile* _modTile)         { modTile = _modTile, name = _modTile->GetName(); }
ATileSide* UTileSideHandler::CurrentSide()              const { return (*this)[currentIndex]; }
ETileColor UTileSideHandler::Color()                    const { return CurrentSide()->ModTile()->GetColor(); }
AModTile*  UTileSideHandler::ModTile()                  const { return modTile; }
TArray<ATileModule*> UTileSideHandler::CurrentModules() const { return CurrentSide()->Modules(); }

void UTileSideHandler::InitModules() const
{
    for (const auto side : GetSides())
        for (const auto module : side->Modules())
            module->Init();
}
TArray<ATileSide*> UTileSideHandler::GetSides() const
{
    TArray<ATileSide*> sides;
    for (const auto child : GetOwner()->Children)
        if (auto* side = static_cast<ATileSide*>(child))
            sides.Add(side);
    return sides;
}
ATileSide* UTileSideHandler::operator[](const int index) const { return index < GetOwner()->Children.Num() ? Cast<ATileSide>(GetOwner()->Children[index]) : nullptr; }
void UTileSideHandler::PropagateSideScale(const FVector scale) const
{
    for (const auto child : GetOwner()->Children)
       child->SetActorScale3D(scale);
}
void UTileSideHandler::PropagateSideLocation(const FVector location) const
{
    for (const auto child : GetOwner()->Children)
        child->SetActorLocation(location);
}
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
    (*this)[index]->K2_DestroyActor();
    currentIndex--;
}
void UTileSideHandler::AddSide(ETileColor color) const
{
    auto* side = GetWorld()->SpawnActor(ATileSide::StaticClass());
    side->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);
    side->SetOwner(GetOwner());
}
bool UTileSideHandler::HasModule(const EModule module)         const { return CurrentSide()->HasModule(module); }
ATileModule* UTileSideHandler::GetModule(const EModule module) const { return CurrentSide()->GetModule(module); }

void UTileSideHandler::SetToDefault() { ChangeSide(0); }
void UTileSideHandler::NextSide()     { ChangeSide(currentIndex + 1); }
void UTileSideHandler::PrevSide()     { ChangeSide(currentIndex - 1); }
void UTileSideHandler::ChangeSide(const int newSide, const bool shouldHide)
{
    // if (shouldHide)
        // (*this)[currentIndex]->SetVisibility(false);
    currentIndex = newSide % SideCount() - 1;
    
    // (*this)[currentIndex]->SetVisibility(true);
}