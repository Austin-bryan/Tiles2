#include "Populators/HexBoardPopulator.h"
#include "HexCoord.h"

void HexBoardPopulator::Populate(const FCoordPtr size, Tiles& tiles)
{
	const auto adjustedSize = FHexCoord(size->X() - 1, size->Y() - 1, size->Z() - 1);
	
	const float posX = adjustedSize.X() / 2 + (fmod(adjustedSize.X(), 2) == 0 ? 0 : 0.5f);
	const float posY = adjustedSize.Y() / 2 + (fmod(adjustedSize.Y(), 2) == 0 ? 0 : 0.5f);
	const float posZ = adjustedSize.Z() / 2 + (fmod(adjustedSize.Z(), 2) == 0 ? 0 : 0.5f);

	for (float x = -posX; x <= posX; x++)
	for (float y = -posY; y <= posY; y++)
	for (float z = -posZ; z <= posZ; z++)
		if (x + y + z == 0)
			CreateTile(MakeShared<FHexCoord>(x, y, z), tiles);
}