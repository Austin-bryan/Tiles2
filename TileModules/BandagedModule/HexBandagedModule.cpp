#include "HexBandagedModule.h"
#include "Board.h"
#include "CoordHeaders.h"
#include "TilesMap.h"
#include "ForwardDeclares.h"
#include "ParameterParseState.h"

void UHexBandagedModule::ApplyParameters(const TArray<FParameter>& parameters)
{
	Tiles& tiles		 = ModTile->Board()->GetTiles();
	const FCoordPtr min  = parameters[0].Get<FCoordPtr>();
	const FCoordPtr mid  = parameters[1].Get<FCoordPtr>();
	const FCoordPtr max  = parameters[2].Get<FCoordPtr>();
	FCoordPtr currCoord  = min;
	FCoordPtr layerStart = min;

	const FCoordPtr distance = FCoord::Distance(min, max);

	// First layer needs to keep one tile, so this skips it	
	int x = 1;
	EDirection topLayer = EDirection::DownRight;

	if (max->Z() == min->Z())
	{	// Same Z layer
		if (max->X() > min->X())
			topLayer = EDirection::DownRight;
		else if (max->X() < min->X())
			topLayer = EDirection::UpLeft;
	}
	else if (max->Y() == min->Y())
	{	// Same Y layer
		if (max->X() < min->X())
			topLayer = EDirection::DownLeft;
		else if (max->X() > min->X())
			topLayer = EDirection::UpRight;
	}

	currCoord += topLayer;

	for (int z = 0; z <= distance->Z(); z++)
	{
		for (; x <= distance->X(); x++)
		{
			if (!tiles.Contains(currCoord))
			{
				Log("tile out of bounds");
				return;
			}
			
			// tiles[currCoord]->Destroy();
			// tiles[currCoord] = ModTile;
			tiles[currCoord]->SetActorScale3D(FVector(0.8f, 0.8f, 0.8f));

			if (*currCoord == *max)
				return;
			else
			{
				Log("Not Equal: "_f + currCoord->ToString() + PAIR + max->ToString(), FColor::Black);
			}
			currCoord += topLayer;
		}
		layerStart += EDirection::Down;
		currCoord = layerStart;
		x = 0;
	}
			ModTile->SetActorScale3D(FVector(1.2f, 1.2f, 1.2f));
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