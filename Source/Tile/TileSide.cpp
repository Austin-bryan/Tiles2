#include "TileSide.h"
#include "TileColor.h"
#include "Enums.h"
#include "ModTile.h"

ETileColor UTileSide::Color() const                     { return ETileColor::Black; }
AModTile* UTileSide::ModTile() const                    { return modTile;}
UTileModule* UTileSide::GetModule(const EModule module) { return moduleMap[module]; }
bool UTileSide::HasModule(const EModule module) const   { return moduleMap.Contains(module); }
TArray<UTileModule*> UTileSide::Modules() const
{
    TArray<UTileModule*> valueArray;
    moduleMap.GenerateValueArray(valueArray);
    return valueArray;
}

void UTileSide::SetColor(const ETileColor _color) { color = _color, ModTile()->SetColor(_color); }
void UTileSide::SetModTile(AModTile* _modTile)    { modTile = _modTile; }
void UTileSide::BeginPlay()                       { Super::BeginPlay(); }

//TODO:: add enum to module field to make sure that it wasnt already added
void UTileSide::AddModule(UTileModule* module)     { moduleMap.Add(module); }
void UTileSide::RemoveModule(const EModule module) { moduleMap.Remove(module); }
void UTileSide::RemoveAll()                        { moduleMap.Empty(); }