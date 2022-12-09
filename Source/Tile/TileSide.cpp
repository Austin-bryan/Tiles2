#include "TileSide.h"
#include "TileColor.h"
#include "Enums.h"
#include "Logger.h"
#include "ModTile.h"

ETileColor ATileSide::Color() const                     { return ETileColor::Black; }
AModTile* ATileSide::ModTile() const                    { return modTile;}

// TODO:: Make this generic and return the generic type
ATileModule* ATileSide::GetModule(const EModule module) { return moduleMap.Contains(module) ? moduleMap[module] : nullptr; }
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

// void ATileSide::SetColor(const ETileColor _color) { color = _color, ModTile()->SetColor(_color, ); }
void ATileSide::SetModTile(AModTile* _modTile)    { modTile = _modTile; }
void ATileSide::BeginPlay()                       { Super::BeginPlay(); }

void ATileSide::AddModule(ATileModule* module)
{
    Path(1);
    if (moduleMap.Contains(module->ModuleType()))
        return;
    Path(2);
    module->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
    module->SetOwner(this);
    module->Init();
    moduleMap.Add(module->ModuleType(), module);
}
void ATileSide::RemoveModule(const EModule module)
{
    moduleMap[module]->Destroy();
    moduleMap.Remove(module);
}
void ATileSide::RemoveAll()
{
    const auto modules = Modules();
    for (const auto module : modules)
        RemoveModule(module->ModuleType());
}