#include "TileSide.h"
#include "TileColor.h"
#include "Enums.h"
#include "ModTile.h"

ETileColor ATileSide::Color() const                     { return ETileColor::Black; }
AModTile* ATileSide::ModTile() const                    { return modTile;}
UTileModule* ATileSide::GetModule(const EModule module) { return moduleMap[module]; }
bool ATileSide::HasModule(const EModule module) const   { return moduleMap.Contains(module); }
TArray<UTileModule*> ATileSide::Modules() const
{
    TArray<UTileModule*> valueArray;
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

//TODO:: add enum to module field to make sure that it wasnt already added
void ATileSide::AddModule(UTileModule* module)     { moduleMap.Add(module->ModuleType(), module); }
void ATileSide::RemoveModule(const EModule module) { moduleMap.Remove(module); }
void ATileSide::RemoveAll()                        { moduleMap.Empty(); }