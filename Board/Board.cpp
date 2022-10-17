#include "Board/Board.h"

ABoard::ABoard()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}
void ABoard::BeginPlay()
{
	Super::BeginPlay();
	Parser parser(this, BoardSeed);
	parser.Parse(Shape, Size, tiles);
}

float ABoard::GetCenteredPosition(const float coord) { return coord / 2; }

void ABoard::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	//parser->PrintStack();
}