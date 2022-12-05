#include "TileModule.h"
#include "ModTile.h"

template<class T>
ATileModule* ATileModule::Create(AModTile* tile, const TArray<FParameter>& parameters)
{
	//TODO:: assert generic is right class
	ATileModule* module = Cast<T>(tile->GetWorld()->SpawnActor(T::StaticClass()));
	module->ModTile = tile;
	module->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	module->ApplyParameters(parameters);
	
	return module;
}

ATileModule::ATileModule()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}
void ATileModule::BeginPlay() { Super::BeginPlay(); }
void ATileModule::Init() const { }