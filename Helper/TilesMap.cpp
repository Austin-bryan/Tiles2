#pragma once
#include "TilesMap.h"
#include "Coord.h"

ATile*&       Tiles::operator[](const FCoordPtr index)       { return map[FCoord::GetMembers(index)]; }
ATile* const& Tiles::operator[](const FCoordPtr index) const { return map[FCoord::GetMembers(index)]; }

TArray<FCoordPtr> Tiles::Keys() const 
{
    TArray<FCoordPtr> keys;
    sliceMap.GenerateValueArray(keys);
    return keys;
}
TArray<ATile*> Tiles::Values() const
{
    TArray<ATile*> tiles;
    map.GenerateValueArray(tiles);
    return tiles;
}

void Tiles::Add(const FCoordPtr coord, ATile* tile) { map.Add(FCoord::GetMembers(coord), tile); }
bool Tiles::Contains(const FCoordPtr coord) const
{
    return map.Contains(FCoord::GetMembers(coord));
}
