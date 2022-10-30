#include "SqrBandagedModule.h"
#include "Board.h"
#include "CoordHeaders.h"
#include "TilesMap.h"
#include "ForwardDeclares.h"
#include "ParameterParseState.h"
#include "Logger.h"

void USqrBandagedModule::ApplyParameters(const TArray<FParameter>& parameters)
{
	// Tiles& tiles = ModTile->Board()->GetTiles();
	//
	// if (!parameters[0].GetIf<FCoordPtr>())
	// 	return;
	// const FCoordPtr min  = parameters[0].Get<FCoordPtr>();
	// const FCoordPtr max  = parameters[1].Get<FCoordPtr>();
	// FCoordPtr currCoord  = min;
	// FCoordPtr layerStart = min;
	//
	// const FCoordPtr distance = FCoord::Distance(min, max);
	//
	// // First layer needs to keep one tile, so this skips it	
	// int x = 1;
	// currCoord += EDirection::Right;
	//
	// for (int z = 0; z <= distance->Z(); z++)
	// {
	// 	for (; x <= distance->X(); x++)
	// 	{
	// 		if (!tiles.Contains(currCoord))
	// 		{
	// 			Log("tile out of bounds");
	// 			return;
	// 		}
	// 		
	// 		tiles[currCoord]->Destroy();
	// 		tiles[currCoord] = ModTile;
	// 		currCoord += EDirection::Right;
	// 	}
	// 	layerStart += EDirection::Down;
	// 	currCoord = layerStart;
	// 	x = 0;
	// }
	// ModTile->SetActorScale3D(FVector(1, distance->X() + 1, distance->Z() + 1));
	// ModTile->SetCoord
	// (
	// 	MakeShared<FSqrCoord>
	// 	(
	// 		Average(min->X(), max->X()),
	// 		Average(min->Z(), max->Z())
	// 	)
	// );
}
