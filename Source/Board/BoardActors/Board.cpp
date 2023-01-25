#include "Board.h"
#include "Parser.h"
#include "Coord.h"

ABoard::ABoard()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}
FVector ABoard::LocationOf(const FCoordPtr coord) const
{
	const FVector forward   = GetActorForwardVector();
	const FVector positionZ = FVector(0, 0, coord->GetOffsetZ() * coord->GetSpaceZ());
	const FVector positionX = forward * coord->GetOffsetX() * coord->GetSpaceX();
	
	return GetActorLocation() - positionX + positionZ;
}

void ABoard::BeginPlay()
{
	Super::BeginPlay();
	Parser parser(this, BoardSeed);
	parser.Parse(Shape, Size, tiles);
}
float ABoard::GetCenteredPosition(const float coord) { return coord / 2; }
void ABoard::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (!UseCustomSeed)
		BoardSeed = Shape == EBoardShape::Square ? ToString(SquareSeed)
				  : Shape == EBoardShape::Hex ? ToString(HexSeed)
				  : ToString(TriangleSeed);
}
float GetMax(const float n)              { return FMath::Abs(n) * 2; }
FCoord ABoard::MinBounds() const         { return FCoord(0, 0, 0); }
FCoord ABoard::MaxBounds() const         { return FCoord(GetMax(Size->X()), GetMax(Size->Y()), GetMax(Size->Z())); }
void ABoard::Tick(const float deltaTime) { Super::Tick(deltaTime); }
