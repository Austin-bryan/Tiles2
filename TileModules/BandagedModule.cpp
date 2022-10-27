#include "Board.h"
#include "Coord.h"
#include "HexCoord.h"
#include "SqrCoord.h"
#include "TilesMap.h"
#include "ForwardDeclares.h"
#include "ParameterParseState.h"
#include "TileModules/BandagedModule.h"

float average(const float a, const float b) { return (a + b) / 2; }

void UBandagedModule::ApplyParameters(const TArray<FParameter>& parameters)
{
	Tiles& tiles		 = ModTile->Board()->GetTiles();
	const FCoordPtr min  = parameters[0].Get<FCoordPtr>();
	const FCoordPtr max  = parameters[parameters.Num() - 1].Get<FCoordPtr>();
	FCoordPtr currCoord  = min;
	FCoordPtr layerStart = min;

	float scaleX = 1;

	// float lengthX = 
	
	do
	{
		currCoord += EDirection::Right;
			
		if (currCoord->X() > ModTile->Board()->MaxBounds().X())
		{
			layerStart += EDirection::Down;
			if (layerStart->Z() > ModTile->Board()->MaxBounds().Z()) 
				return;
			currCoord = layerStart;
		}

		if (!tiles.Contains(currCoord))
		{
			const auto& keys = tiles.Keys();

			// Log("count: "_f + fstr(keys.Num()) + SPC + fstr(tiles.Num()));
			for (int i = 0; i < keys.Num(); i++)
			{
				// Log(keys[i]->ToString() + " does not equal "_f + currCoord->ToString());
			}
			// Log("no contain: "_f + currCoord->ToString());
			return;
		}
		tiles[currCoord]->Destroy();
		tiles[currCoord] = ModTile;
		
		scaleX++;
		ModTile->SetActorScale3D(FVector(1, scaleX, 1));
		ModTile->SetCoord(
			MakeShared<FSqrCoord>(
				average(min->X(), max->X()),
				average(min->Z(), max->Z())
			)
		);
	}
	while (currCoord->X() <= max->X());
}

void UBandagedModule::BeginPlay() { Super::BeginPlay(); }
void UBandagedModule::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}