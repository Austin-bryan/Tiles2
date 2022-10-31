#include "Tile.h"
#include "AssetDir.h"
#include "Board.h"
#include "Logger.h"
#include "Enums.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"
#include "TileModule.h"
#include "TileColor.h"

//#define ShowDebugText
#ifdef ShowDebugText
#include "Coord.h"
#endif 

int ATile::tileCount = 0;

ATile::ATile()
{
	id = tileCount++;
	
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetWorldRotation(FRotator(0, -90, 0));
	RootComponent = Mesh;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

#ifdef ShowDebugText
	CoordText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Coord Text"));
	if (!CoordText)
		return;
	CoordText->SetText(FText::FromString(TEXT("swag")));
	CoordText->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	CoordText->SetRelativeLocation(FVector::ZeroVector + GetActorForwardVector() - 17);
	CoordText->SetRelativeRotation(FRotator(0, 0, 0));
	CoordText->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	CoordText->SetWorldSize(18);
#endif
}

void ATile::BeginPlay() { Super::BeginPlay(); }
void ATile::SetColor(const ETileColor color)
{
	const FString path = "MaterialInstanceConstant'/Game/Materials/TileColors/MI_TileColor.MI_TileColor'"_f;

	if (mat == nullptr)
	{
		mat = Cast<UMaterialInstanceConstant>(
		StaticLoadObject(UMaterialInstanceConstant::StaticClass(), nullptr, *path));
		const FMaterialParameterInfo info("Color");
		instance = UMaterialInstanceDynamic::Create(mat, this);
	}
	instance->SetVectorParameterValue(FName("Color"), UColorCast::TileColorToLinearColor(color));
	if (instance == nullptr)
	{
		Log("null instance");
		return;
	}
	if (Mesh == nullptr)
	{
		Log("null mesh");
		return;
	}
	Mesh->SetMaterial(0, instance);
}
void ATile::SetBoard(ABoard* newBoard)
{
	if (board == nullptr)
		board = newBoard;
}
void ATile::Tick    (const float DeltaTime)
{
	Super::Tick(DeltaTime);
#ifdef UE_BUILD_DEBUG
#ifdef ShowDebugText
	CoordText->SetWorldRotation(FRotator(0, 90, 0));
	CoordText->SetText(FText::FromString(Coord->ToString() + "\n"_f + fstr(id)));
#endif
#endif
}
void ATile::SetCoord(const FCoordPtr coord)
{
	Coord = coord;
#ifdef ShowDebugText
	CoordText->SetText(FText::FromString(coord->ToString() + "\n"_f + fstr(id)));
#endif
	SetActorLocation(board->LocationOf(coord));
}
void ATile::SetShape(const EBoardShape boardShape) const
{
	FString meshDir;
	switch (boardShape)
	{
	case EBoardShape::Square:   meshDir = SQUARE_TILE;   break;
	case EBoardShape::Triangle: meshDir = TRIANGLE_TILE; break;
	case EBoardShape::Hex:      meshDir = HEX_TILE;      break;
	}
	const auto tileMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *meshDir));
	Mesh->SetStaticMesh(tileMesh);
}

void ATile::NotifyActorOnClicked(FKey buttonPressed) 
{
	//Log(*Coord);
}