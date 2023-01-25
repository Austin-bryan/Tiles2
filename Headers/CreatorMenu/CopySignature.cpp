#include "CopySignature.h"
#include "Tile.h"

CopySignature::CopySignature(ATile* _tile)
{
    color = _tile->GetColor();
    tile = _tile;
}