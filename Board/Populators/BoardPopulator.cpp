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
ATile* BoardPopulator::CreateTile(const FCoordPtr coord, Tiles& tiles) const
{
	const auto tile = board->GetWorld()->SpawnActor<ATile>(ATile::StaticClass(), board->LocationOf(coord), FRotator(0, 90, 0));
	tiles.Add(coord, tile);
	tile->SetShape(boardShape);
	tile->SetBoard(board);
	tile->SetCoord(coord);
	
	return tile;
}
