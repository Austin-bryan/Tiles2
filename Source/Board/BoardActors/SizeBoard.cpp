#include "SizeBoard.h"
#include "SizeTile.h"

UClass* ASizeBoard::TileClass() const { return ASizeTile::StaticClass(); }
