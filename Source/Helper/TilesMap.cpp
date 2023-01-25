#pragma once
#include "TilesMap.h"
#include "Coord.h"
#include "Logger.h"

ATile*&       TilesMap::operator[](const FCoordPtr index)       { return map[*index]; }
ATile* const& TilesMap::operator[](const FCoordPtr index) const { return map[*index]; }

ATile*&       TilesMap::operator[](const FCoord& index)       { return map[index]; }
ATile* const& TilesMap::operator[](const FCoord& index) const { return map[index]; }

TArray<ATile*> TilesMap::Values() const
{
    TArray<ATile*> tiles;
    map.GenerateValueArray(tiles);
    return tiles;
}
void TilesMap::Add(const FCoordPtr coord, ATile* tile) { map.Add(*coord, tile); }
bool TilesMap::Contains(const FCoordPtr coord) const   { return map.Contains(*coord); }
bool TilesMap::Contains(const FCoord& coord) const     { return map.Contains(coord); }
void TilesMap::Empty() { map.Empty(); }