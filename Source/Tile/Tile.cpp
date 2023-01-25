#include "Tile.h"
#include "AssetDir.h"
#include "Board.h"
#include "Enums.h"
#include "TileColor.h"
#include "MeshGenerator.h"
#include "ProceduralMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

#ifdef SHOW_DEBUG_TEXT
#include "Coord.h"
#include "Components/TextRenderComponent.h"
#endif 

int ATile::tileCount = 0;

ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	id   = tileCount++;
	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	Root->SetRelativeLocation(FVector::ZeroVector);
	RootComponent = Root;
	
	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh"));
	ProcMesh->SetupAttachment(Root);
	ProcMesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	MeshGenerator = CreateDefaultSubobject<UMeshGenerator>(TEXT("Mesh Generator"));
	MeshGenerator->PrimaryComponentTick.bCanEverTick = true;
	MeshGenerator->PrimaryComponentTick.bStartWithTickEnabled = true;
	MeshGenerator->SetComponentTickEnabled(true);
	MeshGenerator->RegisterComponent();
	MeshGenerator->ProceduralMesh = ProcMesh;

	// By avoiding using the mesh as the collider, several scale related bugs are avoided
	Collider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collider"));
	Collider->SetWorldRotation(FRotator(0, -90, 0));
	Collider->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	Collider->SetCollisionProfileName("OverlapAll");
	Collider->SetVisibility(false);

#ifdef SHOW_DEBUG_TEXT
	CoordText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Coord Text"));
	CoordText->SetText(FText::FromString(TEXT("swag")));
	CoordText->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	CoordText->SetWorldSize(18);
	CoordText->VerticalAlignment   = EVRTA_TextCenter;
	CoordText->HorizontalAlignment = EHTA_Center;
	CoordText->SetRelativeLocation(FVector::ZeroVector + GetActorForwardVector() + 17);
#endif
}
void ATile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
#ifdef SHOW_DEBUG_TEXT
	CoordText->SetWorldRotation(FRotator(0, 90, 0));
#endif
}

// TODO:: This is a copy-paste from Vertex::IsOnBoardEdge(). Reuse the code somehow
bool ATile::IsOnBoardEdge() const
{
	int neighborCount = GetNeighbors().Num();
	
	return Shape() == EBoardShape::Triangle && neighborCount != 3
		|| Shape() == EBoardShape::Square 	&& neighborCount != 4
		|| Shape() == EBoardShape::Hex    	&& neighborCount != 6;
}
ATile* ATile::GetNeighbor(EDirection direction) const
{
	auto neighborCoord = GetCoord() + direction;
	return board->Contains(neighborCoord) ? board->At(GetCoord() + direction) : nullptr;
}
void ATile::SetColor(ETileColor color, bool propagate) { ApplyColor(color, propagate); }
void ATile::ApplyColor(ETileColor color, bool propagate)
{
	const FString path = "MaterialInstanceConstant'/Game/Materials/TileColors/MI_TileColor.MI_TileColor'"_f;
	
	if (instance == nullptr)
	{
		const auto mat = Cast<UMaterialInstanceConstant>(
		StaticLoadObject(UMaterialInstanceConstant::StaticClass(), nullptr, *path));
		instance = UMaterialInstanceDynamic::Create(mat, this);
	}
	tileColor = color;
	instance->SetVectorParameterValue(FName("Color"), UColorCast::TileColorToLinearColor(color));
	
	MeshGenerator->ProceduralMesh->SetMaterial(0, instance);
}
TArray<ATile*> ATile::GetNeighbors() const
{
	TArray<ATile*> tiles;
	const auto adjacentCoords = Coord->GetAdjacent(board);

	for (const auto coord : adjacentCoords)
		tiles.Add(board->GetTiles()[coord]);
	return tiles;
}

// TODO:: make this a field be held by tileside
ETileColor ATile::GetColor() 			   const { return tileColor; }
EBoardShape ATile::Shape()   			   const { return Board()->GetBoardShape(); }
bool ATile::IsAdjacent(const ATile* other) const { return Coord->IsAdjacent(other->Coord); }

void ATile::SetBoard(ABoard* newBoard)
{
	if (board != nullptr)
		return;
	board = newBoard;
	AttachToActor(board, FAttachmentTransformRules::KeepRelativeTransform);
}
void ATile::SetCoord(const FCoordPtr coord)
{
	Coord = coord;
	SetDebugText();
	SetActorLocation(board->LocationOf(coord));
}
void ATile::SetDebugText() const
{
#ifdef SHOW_DEBUG_TEXT
	FString text;
#ifdef SHOW_COORD
	text = text + Coord->ToString() + "\n"_f;
#endif
#ifdef SHOW_ID
	text = text + id;
#endif
	CoordText->SetText(ftxt(text));
	CoordText->SetRelativeLocation(FVector(0, 10, 0));
#endif
}

void ATile::SetShape(const EBoardShape boardShape) const
{
	FString meshDir;
	switch (boardShape)
	{
	case EBoardShape::Square:
		meshDir = SQUARE_TILE;
		MeshGenerator->Init(70, 4, 45, 90);
		break;
	case EBoardShape::Triangle:
		meshDir = TRIANGLE_TILE;
		MeshGenerator->Init(70, 3, -30, 120);
		break;
	case EBoardShape::Hex:
		meshDir = HEX_TILE; 
		MeshGenerator->Init(55, 6, 0, 60);
		break;
	}
	const auto tileMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *meshDir));
	Collider->SetStaticMesh(tileMesh);
}
void ATile::NotifyActorOnClicked(FKey buttonPressed) { }