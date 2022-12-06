#include "TileSide.h"
#include "TileColor.h"
#include "Enums.h"
#include "ModTile.h"

ETileColor ATileSide::Color() const                     { return ETileColor::Black; }
AModTile* ATileSide::ModTile() const                    { return modTile;}
ATileModule* ATileSide::GetModule(const EModule module) { return moduleMap[module]; }
bool ATileSide::HasModule(const EModule module) const   { return moduleMap.Contains(module); }
TArray<ATileModule*> ATileSide::Modules() const
{
    TArray<ATileModule*> valueArray;
    moduleMap.GenerateValueArray(valueArray);
    return valueArray;
}

ATileSide::ATileSide()
{
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);
}

void ATileSide::SetColor(const ETileColor _color) { color = _color, ModTile()->SetColor(_color); }
void ATileSide::SetModTile(AModTile* _modTile)    { modTile = _modTile; }
void ATileSide::BeginPlay()                       { Super::BeginPlay(); }

void ATileSide::AddModule(ATileModule* module)
{
    if (moduleMap.Contains(module->ModuleType()))
        return;
    module->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
    module->SetOwner(this);
    module->Init();
    moduleMap.Add(module->ModuleType(), module);
}
void ATileSide::RemoveModule(const EModule module) { moduleMap.Remove(module); }
void ATileSide::RemoveAll()                        { moduleMap.Empty(); }