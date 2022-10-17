#include "SquareBoardPopulator.h"
#include "Coord/SquareCoord.h"
#include "Board.h"

void SquareBoardPopulator::Populate(FCoord* size, FTiles& tiles) 
{
	const float posX = size->X / 2;
	const float posZ = size->Z / 2;

	// TArray<FSquareCoord> ignoreCoords1 { FSquareCoord(0,0), FSquareCoord(1,1), FSquareCoord(1,0), FSquareCoord(0,1) };
	// TArray<FSquareCoord> ignoreCoords2 { FSquareCoord(-2,-1), FSquareCoord(-1,-1), FSquareCoord(0,-1), FSquareCoord(1,-1) };

	for (float z = -posZ + 0.5f; z < posZ; z++)
	for (float x = -posX + 0.5f; x < posX; x++)
	{
		const auto coord = new FSquareCoord(x, z);

		//if (ignoreCoords1.Contains(*coord))
		//	continue;
		//if (ignoreCoords2.Contains(*coord))
		//	continue;
		CreateTile(coord, tiles);
	}
	//int count = 0;
	//float zSum = 0, xSum = 0;
	//for (const FSquareCoord& c : ignoreCoords1)
	//{
	//	count++;
	//	zSum += c.Z;
	//	xSum += c.X;
	//}
	//auto tile = CreateTile(new FSquareCoord(xSum / count, zSum / count));
	//tile->SetActorScale3D(FVector(1, 2, 2));
	//count = 0;
	//zSum = 0, xSum = 0;
	//for (const FSquareCoord& c : ignoreCoords2)
	//{
	//	count++;
	//	zSum += c.Z;
	//	xSum += c.X;
	//}
	//tile = CreateTile(new FSquareCoord(xSum / count, zSum / count));
	//tile->SetActorScale3D(FVector(1, 4.25f, 1));
}

float SquareBoardPopulator::GetOffsetX(FCoord* coord) const { return -coord->X; }
float SquareBoardPopulator::GetOffsetZ(FCoord* coord) const { return -coord->Z; }
