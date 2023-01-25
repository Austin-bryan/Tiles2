#include "SqrBoardPopulator.h"
#include "SqrCoord.h"

void SqrBoardPopulator::Populate(const FCoordPtr size, TilesMap& tiles) 
{
	const float posX = size->X() / 2;
	const float posZ = size->Z() / 2;


	for (float z = -posZ + 0.5f; z <  posZ; z++)
	for (float x = -posX + 0.5f; x < posX; x++)
	{
		const auto coord = MakeShared<FSqrCoord>(x, z);
		CreateTile(coord, tiles);
	}
}
