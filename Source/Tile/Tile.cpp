#include "Tile.h"
#include "AssetDir.h"
#include "Board.h"
#include "Logger.h"
#include "Enums.h"
#include "TileModule.h"
#include "TileColor.h"
#include "MeshGenerator.h"
#include "Materials/MaterialInstanceConstant.h"

//#define ShowDebugText
#ifdef ShowDebugText
#include "Coord.h"
#endif 

int ATile::tileCount = 0;

ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	id   = tileCount++;
	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	Root->SetRelativeLocation(FVector::ZeroVector);
	RootComponent = Root;
	
	// Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	// Mesh->SetWorldRotation(FRotator(0, -90, 0));
	// Mesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	// Mesh->SetCollisionProfileName("BlockAll");
	// Mesh->SetGenerateOverlapEvents(false);
	// Mesh->SetWorldScale3D(FVector(0.1f));

	MeshGenerator = CreateDefaultSubobject<UMeshGenerator>(TEXT("Mesh Generator"));
	MeshGenerator->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	MeshGenerator->PrimaryComponentTick.bCanEverTick = true;
	MeshGenerator->PrimaryComponentTick.bStartWithTickEnabled = true;
	MeshGenerator->SetComponentTickEnabled(true);
	MeshGenerator->SetupAttachment(Root);
	MeshGenerator->RegisterComponent();
	MeshGenerator->ProceduralMesh->SetupAttachment(Root);
	MeshGenerator->ProceduralMesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

	// By avoiding using the mesh as the collider, several scale related bugs are avoided
	Collider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collider"));
	Collider->SetWorldRotation(FRotator(0, -90, 0));
	Collider->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	Collider->SetCollisionProfileName("OverlapAll");
	Collider->SetVisibility(false);

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

void ATile::BeginPlay()
{
	Super::BeginPlay();
}
void ATile::SetColor(const ETileColor color)
{
	const FString path = "MaterialInstanceConstant'/Game/Materials/TileColors/MI_TileColor.MI_TileColor'"_f;

	if (instance == nullptr)
	{
		const auto mat = Cast<UMaterialInstanceConstant>(
		StaticLoadObject(UMaterialInstanceConstant::StaticClass(), nullptr, *path));
		instance = UMaterialInstanceDynamic::Create(mat, this);
	}
	instance->SetVectorParameterValue(FName("Color"), UColorCast::TileColorToLinearColor(color));
	// Mesh->SetMaterial(0, instance);
	MeshGenerator->ProceduralMesh->SetMaterial(0, instance);
}
void ATile::SetBoard(ABoard* newBoard)
{
	if (board != nullptr)
		return;
	board = newBoard;
	AttachToActor(board, FAttachmentTransformRules::KeepRelativeTransform);
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
	// Mesh->SetStaticMesh(tileMesh);
	Collider->SetStaticMesh(tileMesh);
}
void ATile::NotifyActorOnClicked(FKey buttonPressed) 
{
	//Log(*Coord);
}