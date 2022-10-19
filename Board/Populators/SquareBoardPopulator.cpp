#include "SquareBoardPopulator.h"
#include "Coord/SquareCoord.h"

void SquareBoardPopulator::Populate(const FCoordPtr size, Tiles& tiles) 
{
	const float posX = size->X() / 2;
	const float posZ = size->Z() / 2;

	for (float z = -posZ + 0.5f; z < posZ; z++)
	for (float x = -posX + 0.5f; x < posX; x++)
	{
		const auto coord = MakeShared<FSquareCoord>(x, z);
		CreateTile(coord, tiles);
	}
}

float SquareBoardPopulator::GetOffsetX(const FCoordPtr coord) const { return -coord->X(); }
float SquareBoardPopulator::GetOffsetZ(const FCoordPtr coord) const { return -coord->Z(); }
