#include "Populators/BoardPopulator.h"
#include "Kismet/GameplayStatics.h"
#include "Board.h"
#include "Tile.h"

BoardPopulator::BoardPopulator(ABoard* const board) : board{board}, boardShape{ board->GetBoardShape() } 
{ 
	const auto controller = board->GetWorld()->GetFirstPlayerController();
	controller->bShowMouseCursor       = true;
	controller->bEnableTouchEvents     = true;
	controller->bEnableClickEvents     = true;
	controller->bEnableMouseOverEvents = true;
}
ATile* BoardPopulator::CreateTile(const FCoordPtr coord, Tiles& tiles) const
{
	auto d = ATile::StaticClass();
	const auto tile = board->GetWorld()->SpawnActor<ATile>(
		board->TileClass(),
		board->LocationOf(coord),
		FRotator(0, 0, 0));
	tiles.Add(coord, tile);
	tile->SetShape(boardShape);
	tile->SetBoard(board);
	tile->SetCoord(coord);
	
	return tile;
}