#include "TileModules/TileModule.h"
#include "Components/StaticMeshComponent.h"
#include "AssetDir.h"

UTileModule::UTileModule()
{
	PrimaryComponentTick.bCanEverTick = true;
}
UTileModule::~UTileModule() {}

template<class T>
UTileModule* UTileModule::Create(ATile* tile)
{
	//todo:= assert generic is right class
	UTileModule* module = NewObject<UTileModule>(tile, T::StaticClass(), FName(T::StaticClass()->GetName()));
	module->ModTile = tile;
	module->RegisterComponent();
	module->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	module->CreationMethod = EComponentCreationMethod::Instance;
	return module;
}
void UTileModule::BeginPlay()
{
	Super::BeginPlay();
	Log("Begin");
}
void UTileModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}