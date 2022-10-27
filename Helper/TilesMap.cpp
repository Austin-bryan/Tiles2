#pragma once
#include "TilesMap.h"
#include "Coord.h"

ATile*&       Tiles::operator[](const FCoordPtr index)       { return map[index->X()][index->Y()][index->Z()]; }
ATile* const& Tiles::operator[](const FCoordPtr index) const { return (*this)[index]; }
void Tiles::Add(const FCoordPtr coord, ATile* tile)          { map[coord->X()][coord->Y()][coord->Z()] = tile; }

// TArray<FCoordPtr> Tiles::Keys() const 
// {
//     TArray<FCoordPtr> keys;
//     sliceMap.GenerateValueArray(keys);
//     return keys;
// }
TArray<ATile*> Tiles::Values() const
{
    TArray<ATile*> tiles;
    for (const TArray<TArray<ATile*>>& layerY : map)
        for (const TArray<ATile*>& layerZ : layerY)
            for (ATile* tile : layerZ)
                tiles.Add(tile);
    return tiles;
}

bool Tiles::Contains(const FCoordPtr coord) const
{
    if (map.Contains(coord->X()))
        if (map[coord->X()].Contains(coord->Y()))
            if (map[coord->X()][coord->Y()].Contains(coord->Z()))
                return true;
    return false;
}
