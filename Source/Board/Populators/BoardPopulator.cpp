#include "Populators/BoardPopulator.h"
#include "Board.h"
#include "Tile.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

BoardPopulator::BoardPopulator(ABoard* const board) : board{board}, boardShape{ board->GetBoardShape() } 
{ 
	const auto controller = board->GetWorld()->GetFirstPlayerController();
	controller->bShowMouseCursor       = true;
	controller->bEnableTouchEvents     = true;
	controller->bEnableClickEvents     = true;
	controller->bEnableMouseOverEvents = true;
}
ATile* BoardPopulator::CreateTile(const FCoordPtr coord, TilesMap& tiles) const
{
	ATile* tile = board->GetWorld()->SpawnActor<ATile>(
		board->TileClass(),
		board->LocationOf(coord),
		FRotator::ZeroRotator);
	tiles.Add(coord, tile);
	tile->SetBoard(board);
	tile->SetCoord(coord);
	tile->SetShape(boardShape);
	
	return tile;
}