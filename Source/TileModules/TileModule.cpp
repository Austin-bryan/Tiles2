#include "TileModule.h"
#include "ModTile.h"
#include "PaperSpriteComponent.h"

ATileModule::ATileModule()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->RegisterComponent();
	Sprite->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
}

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
void ATileModule::BeginPlay()
{
	Super::BeginPlay();
	Sprite->SetSprite(GetSprite());
}
void ATileModule::Init() const { }