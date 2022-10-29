#include "PlayerBoard.h"
#include "PlayerTile.h"

UClass* APlayerBoard::TileClass() const { return APlayerTile::StaticClass(); }
