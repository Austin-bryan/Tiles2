#include "TileModule.h"

UTileModule::UTileModule()
{
	PrimaryComponentTick.bCanEverTick = true;
}
UTileModule::~UTileModule() {}

template<class T>
UTileModule* UTileModule::Create(ATile* tile, const TArray<FParameter>& parameters)
{
	//todo:= assert generic is right class
	UTileModule* module = NewObject<UTileModule>(tile, T::StaticClass(), FName(T::StaticClass()->GetName()));
	module->ModTile = tile;
	module->RegisterComponent();
	module->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	module->CreationMethod = EComponentCreationMethod::Instance;
	module->ApplyParameters(parameters);
	
	return module;
}
void UTileModule::BeginPlay()
{
	Super::BeginPlay();
}
void UTileModule::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}