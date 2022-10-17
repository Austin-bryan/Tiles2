#include "Board/Populators/HexBoardPopulator.h"
#include "Coord/HexCoord.h"

void HexBoardPopulator::Populate(FCoord* size, TMap<FCoord*, ATile*>& tiles)
{
	FHexCoord oldSize = *static_cast<FHexCoord*>(size);
	oldSize = FHexCoord(oldSize.X - 1, oldSize.Y - 1, oldSize.Z - 1);
	
	const float posX = oldSize.X / 2 + (fmod(oldSize.X, 2) == 0 ? 0 : 0.5f);
	const float posY = oldSize.Y / 2 + (fmod(oldSize.Y, 2) == 0 ? 0 : 0.5f);
	const float posZ = oldSize.Z / 2 + (fmod(oldSize.Z, 2) == 0 ? 0 : 0.5f);

	for (float x = -posX; x <= posX; x++)
	for (float y = -posY; y <= posY; y++)
	for (float z = -posZ; z <= posZ; z++)
		if (x + y + z == 0)
			CreateTile(new FHexCoord(x, y, z), tiles);
}

float HexBoardPopulator::GetOffsetX(FCoord* coord) const { return -coord->X + coord->Y + coord->Z; }
float HexBoardPopulator::GetOffsetZ(FCoord* coord) const { return -coord->Z + coord->Y; }
