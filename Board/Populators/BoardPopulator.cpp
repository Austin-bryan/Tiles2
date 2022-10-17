#include "Board/Populators/BoardPopulator.h"
#include "Kismet/GameplayStatics.h"
#include "Coord/Coord.h"
#include "Board/Board.h"
#include "Tile.h"
#include "Enums.h"

BoardPopulator::BoardPopulator(ABoard* const board, const EBoardShape& boardShape) : 
	board{board}, boardShape{ boardShape } 
{ 
	const auto controller = board->GetWorld()->GetFirstPlayerController();
	controller->bShowMouseCursor       = true;
	controller->bEnableTouchEvents     = true;
	controller->bEnableClickEvents     = true;
	controller->bEnableMouseOverEvents = true;
}
ATile* BoardPopulator::CreateTile(FCoord* coord, TMap<FCoord*, ATile*>& tiles)
{
	const auto forward = board->GetActorForwardVector();
	const auto positionX = forward * GetOffsetX(coord) * GetSpaceX();
	const auto positionZ = FVector(0, 0, GetOffsetZ(coord) * GetSpaceZ());
	const auto location  = board->GetActorLocation() + positionX + positionZ;	
	const auto tile = board->GetWorld()->SpawnActor<ATile>(ATile::StaticClass(), location, FRotator::ZeroRotator);

	tiles.Add(coord, tile);
	tile->SetShape(boardShape);
	tile->SetCoord(coord);
	return tile;
}