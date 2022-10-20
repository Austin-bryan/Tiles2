#include "SqrBoardPopulator.h"
#include "Coord/SqrCoord.h"

// todo:: move get offset to coord
// todo\ have get location be a virtual coord method
// todo\ coord position can be acquired directly from coord
// todo\ setting player coord will automatically set player position
void SqrBoardPopulator::Populate(const FCoordPtr size, Tiles& tiles) 
{
	const float posX = size->X() / 2;
	const float posZ = size->Z() / 2;

	for (float z = -posZ + 0.5f; z < posZ; z++)
	for (float x = -posX + 0.5f; x < posX; x++)
	{
		const auto coord = MakeShared<FSqrCoord>(x, z);
		CreateTile(coord, tiles);
	}
}
