#include "Board/Board.h"
#include "Coord.h"
#include "MathUtil.h"
#include "Parser.h"


ABoard::ABoard()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}
FVector ABoard::LocationOf(const FCoordPtr coord) const
{
	const auto forward = GetActorForwardVector();
	const auto positionZ      = FVector(0, 0, coord->GetOffsetZ() * coord->GetSpaceZ());
	const auto positionX = forward * coord->GetOffsetX() * coord->GetSpaceX();
	return GetActorLocation() + positionX + positionZ;
}

void ABoard::BeginPlay()
{
	Super::BeginPlay();
	Parser parser(this, BoardSeed);
	parser.Parse(Shape, Size, tiles);
}
float ABoard::GetCenteredPosition(const float coord) { return coord / 2; }

//todo:: ensure this works with other boardshapes
float GetMax(const float n) { return FMathf::Abs(n) * 2; }
CoordMembers ABoard::MinBounds() const
{
	return CoordMembers(0, 0, 0);
}
CoordMembers ABoard::MaxBounds() const
{
	return CoordMembers(GetMax(Size->X()), GetMax(Size->Y()), GetMax(Size->Z()));
}
void ABoard::Tick(const float deltaTime) { Super::Tick(deltaTime); }
