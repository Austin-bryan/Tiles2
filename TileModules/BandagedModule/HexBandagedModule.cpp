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
	float traveledX = 1, traveledZ = 1;

	do
	{
		currCoord += EDirection::Right;

		// Check if over extending X layer
		if (traveledX > distance->X())
		{
			layerStart += EDirection::Down;

			// Check of over extending Y layer
			if (traveledZ > distance->Z()) 
				return;
			currCoord = layerStart;
			traveledX = 0;
			traveledZ++;
		}
		if (!tiles.Contains(currCoord))
		{
			Log("invalid");
			return;
		}
		
		// Destroy existing tiles and substitute
		tiles[currCoord]->Destroy();
		tiles[currCoord] = ModTile;

		ModTile->SetActorScale3D(FVector(1, distance->X() + 1, distance->Z() + 1));
		ModTile->SetCoord(
			MakeShared<FSqrCoord>(
				Average(min->X(), max->X()),
				Average(min->Z(), max->Z())
			));
		traveledX++;
	}
	while (currCoord->X() <= max->X());
}
