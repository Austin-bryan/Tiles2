#include "Tile.h"
#include "AssetDir.h"
#include "Board.h"
#include "Logger.h"
#include "Enums.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"
#include "TileModules/TileModule.h"

ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	CoordText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Coord Text"));
	if (!CoordText)
		return;
	CoordText->SetText(FText::FromString(TEXT("swag")));
	CoordText->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	CoordText->SetRelativeLocation(FVector::ZeroVector + GetActorForwardVector() * 17);
	CoordText->SetRelativeRotation(FRotator(0, 0, 0));
	CoordText->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	CoordText->SetWorldSize(18);
}

void ATile::SetColor(const ETileColor color) const
{
	const FString path = fstr("MaterialInstanceConstant'/Game/Materials/TileColors/MI_") + TileColorStrings[color] + fstr("Tile.MI_") + TileColorStrings[color] + fstr("Tile'");
	const auto mat = Cast<UMaterialInstanceConstant>(StaticLoadObject(UMaterialInstanceConstant::StaticClass(), nullptr, *path));
	Mesh->SetMaterial(0, mat);
}

void ATile::SetBoard(const ABoard* board)
{
	if (DesignatedBoard == nullptr)
		DesignatedBoard = board;
}
void ATile::BeginPlay() { Super::BeginPlay(); }
void ATile::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
#ifdef UE_BUILD_DEBUG
	CoordText->SetWorldRotation(FRotator(0, 90, 0));
	CoordText->SetText(FText::FromString(Coord->ToString()));
#endif
}
void ATile::SetCoord(const FCoordPtr coord)
{
	Coord = coord;
	CoordText->SetText(FText::FromString(coord->ToString()));
	SetActorLocation(DesignatedBoard->LocationOf(coord));
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