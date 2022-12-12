#include "TileSide.h"
#include "TileColor.h"
#include "Enums.h"
#include "Logger.h"
#include "ModTile.h"

// TODO:: Make this generic and return the generic type of GetModule()

ETileColor ATileSide::Color() const                     { return ETileColor::Black; }
AModTile* ATileSide::ModTile() const                    { return Cast<AModTile>(GetOwner()); }
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
    PrimaryActorTick.bCanEverTick = true;
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);
}
void ATileSide::Tick(const float DeltaSeconds) { Super::Tick(DeltaSeconds); }
void ATileSide::SetModTile(AModTile* _modTile) { modTile = _modTile; }
void ATileSide::BeginPlay()                    { Super::BeginPlay(); }
void ATileSide::AddModule(ATileModule* module)
{
    if (moduleMap.Contains(module->ModuleType()))
        return;
    module->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
    module->SetOwner(this);
    module->Init();
    moduleMap.Add(module->ModuleType(), module);
}
void ATileSide::RemoveModule(const ATileModule* module) { RemoveModule(module->ModuleType()); }
void ATileSide::RemoveModule(const EModule module, const bool shouldDestroy)
{
    if (moduleMap.Contains(module))
    {
        if (moduleMap[module] && shouldDestroy)
            moduleMap[module]->Destroy();
        moduleMap.Remove(module);
    }
}
void ATileSide::RemoveAll()
{
    const auto modules = Modules();
    for (const auto module : modules)
        RemoveModule(module->ModuleType());
}