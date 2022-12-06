#include "TileModule.h"

#include "AssetDir.h"
#include "Logger.h"
#include "ModTile.h"
#include "PaperSpriteComponent.h"

ATileModule::ATileModule()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	Sprite->SetRelativeScale3D(FVector(0.125f));
}

template<class T>
ATileModule* ATileModule::Create(AModTile* tile, const TArray<FParameter>& parameters)
{
	ATileModule* module = Cast<T>(tile->GetWorld()->SpawnActor(T::StaticClass()));
	module->ModTile = tile;
	module->AttachToComponent(tile->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	module->ApplyParameters(parameters);
	
	return module;
}
void ATileModule::BeginPlay()
{
	Super::BeginPlay();
	Sprite->SetSprite(GetSprite());
}
void ATileModule::Init() const { }
FString ATileModule::GetSpritePath() const
{
	auto moduleName = UEnum::GetValueAsName(ModuleType()).ToString();
	moduleName.RemoveFromStart("EModule::"_f);
	return  "/Game/Sprites/Tiles/Normal/"_f + moduleName + "_Sprite."_f + moduleName + "_Sprite"_f ;
}
void ATileModule::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	SetActorScale3D(ModTile->GetActorScale());
}
UPaperSprite* ATileModule::GetSprite() const { return LoadObjectFromPath<UPaperSprite>(FName(GetSpritePath())); }
