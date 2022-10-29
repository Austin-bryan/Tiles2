#include "CreatorBoard.h"
#include "CreatorTile.h"

UClass* ACreatorBoard::TileClass() const { return ACreatorTile::StaticClass(); }
