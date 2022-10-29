#include "BandagedModule.h"

float UBandagedModule::Average(const float a, const float b) { return (a + b) / 2; }

// void UBandagedModule::ApplyParameters(const TArray<FParameter>& parameters)
// {
// 	Tiles& tiles		 = ModTile->Board()->GetTiles();
// 	const FCoordPtr min  = parameters[0].Get<FCoordPtr>();
// 	const FCoordPtr max  = parameters[parameters.Num() - 1].Get<FCoordPtr>();
// 	FCoordPtr currCoord  = min;
// 	FCoordPtr layerStart = min;
//
// 	const FCoordPtr distance = FCoord::Distance(min, max);
// 	float traveledX = 1, traveledZ = 1;
//
// 	do
// 	{
// 		currCoord += EDirection::Right;
//
// 		// Check if over extending X layer
// 		if (traveledX > distance->X())
// 		{
// 			layerStart += EDirection::Down;
//
// 			// Check of over extending Y layer
// 			if (traveledZ > distance->Z()) 
// 				return;
// 			currCoord = layerStart;
// 			traveledX = 0;
// 			traveledZ++;
// 		}
// 		if (!tiles.Contains(currCoord))
// 		{
// 			Log("invalid");
// 			return;
// 		}
// 		
// 		// Destroy existing tiles and substitute
// 		tiles[currCoord]->Destroy();
// 		tiles[currCoord] = ModTile;
//
// 		ModTile->SetActorScale3D(FVector(1, distance->X() + 1, distance->Z() + 1));
// 		ModTile->SetCoord(
// 			MakeShared<FSqrCoord>(
// 				average(min->X(), max->X()),
// 				average(min->Z(), max->Z())
// 			));
// 		traveledX++;
// 	}
// 	while (currCoord->X() <= max->X());
// }}