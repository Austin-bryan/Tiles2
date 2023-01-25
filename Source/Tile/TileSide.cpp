#include "TileSide.h"
#include "ModTile.h"
#include "Logger.h"
#include "TileColor.h"
#include "MeshGenerator.h"
#include "ProceduralMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

void ATileSide::SetColor(ETileColor _tileColor, bool propagate)
{
    tileColor = _tileColor;
    const FString path = "MaterialInstanceConstant'/Game/Materials/TileColors/MI_TileColor.MI_TileColor'"_f;

    if (instance == nullptr)
    {
        const auto mat = Cast<UMaterialInstanceConstant>(
        StaticLoadObject(UMaterialInstanceConstant::StaticClass(), nullptr, *path));
        instance = UMaterialInstanceDynamic::Create(mat, this);
    }
    instance->SetVectorParameterValue(FName("Color"), UColorCast::TileColorToLinearColor(tileColor));
    modTile->MeshGenerator->ProceduralMesh->SetMaterial(0, instance);
}
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
    if (!moduleMap.Contains(module))
        return;
    if (moduleMap[module] && shouldDestroy)
        moduleMap[module]->Destroy();
    moduleMap.Remove(module);
}
void ATileSide::RemoveAll()
{
    const auto modules = Modules();
    for (const auto module : modules)
        RemoveModule(module->ModuleType());
}