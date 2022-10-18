#include "Tile.h"
#include "AssetDir.h"
#include "Logger.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Coord/TriCoord.h"
#include "TileModules/TileModule.h"

//todo:= cyan, black and pink colors dont work
TMap<ETileColor, FString> ATile::TileColorStrings =
{
	{ ETileColor::None,    FString("None") },
	{ ETileColor::White,   FString("White") },
	{ ETileColor::Red,     FString("Red") },
	{ ETileColor::Orange,  FString("Orange") },
	{ ETileColor::Yellow,  FString("Yellow") },
	{ ETileColor::Green,   FString("Green") },
	{ ETileColor::Cyan,    FString("Cyan") },
	{ ETileColor::Blue,    FString("Blue") },
	{ ETileColor::Purple,  FString("Purple") },
	{ ETileColor::Pink,    FString("Pink") },
	{ ETileColor::Magenta, FString("Magenta") },
	{ ETileColor::Black,   FString("Black") }
};

ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	//OnClickEvent.BindUFunction(this, "OnClick");
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	//Box->OnClicked.Add(OnClickEvent);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//Box->bHiddenInGame = false;

	//auto m = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh2"));
	//m->AttachToComponent(Mesh, FAttachmentTransformRules::KeepWorldTransform);

	CoordText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Coord Text"));
	if (!CoordText)
		return;
	CoordText->SetText(FText::FromString(TEXT("swag")));
	CoordText->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	CoordText->SetRelativeLocation(FVector::ZeroVector - GetActorForwardVector() * 17);
	CoordText->SetWorldRotation(FRotator(0, GetActorRotation().Yaw, 0));
	CoordText->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	CoordText->SetWorldSize(18);
}

void ATile::SetColor(ETileColor color)
{
	FString path = fstr("MaterialInstanceConstant'/Game/Materials/TileColors/MI_") + ATile::TileColorStrings[color] + fstr("Tile.MI_") + ATile::TileColorStrings[color] + fstr("Tile'");
	auto mat = Cast<UMaterialInstanceConstant>(StaticLoadObject(UMaterialInstanceConstant::StaticClass(), NULL, *path));
	Mesh->SetMaterial(0, mat);
}
void ATile::BeginPlay() 
{
	Super::BeginPlay(); 
}
void ATile::Tick(float DeltaTime) 
{	
	Super::Tick(DeltaTime); 

#ifdef UE_BUILD_DEBUG
	CoordText->SetWorldRotation(FRotator(0, 180, 0));
	CoordText->SetText(FText::FromString(Coord->ToString()));
#endif
}
void ATile::SetCoord(FCoord* coord)
{
	Coord = coord;
	CoordText->SetText(FText::FromString(coord->ToString()));
}
void ATile::SetShape(EBoardShape boardShape)
{
	FString meshDir;
	switch (boardShape)
	{
	case EBoardShape::Square:   meshDir = SQUARE_TILE;   break;
	case EBoardShape::Triangle: meshDir = TRIANGLE_TILE; break;
	case EBoardShape::Hex:      meshDir = HEX_TILE;      break;
	}
	auto tileMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *meshDir));
	Mesh->SetStaticMesh(tileMesh);


}
void ATile::NotifyActorOnClicked(FKey buttonPressed) 
{
	//Log(*Coord);
}