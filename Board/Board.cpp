#include "Board/Board.h"
#include "Coord.h"
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
void ABoard::Tick(const float deltaTime) { Super::Tick(deltaTime); }