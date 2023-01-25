#pragma once
#include "CoreMinimal.h"
#include "ForwardDeclares.h"
#include "Coord.h"
#include "Tile.h"

/**
 * Uses FCoord as the key as opposed to FCoordPtr for ease of use.
 * Maintains original FCoordPtr's incase GetKeys() are called, to get the original unsliced version
 */
class TilesMap
{
public:
    ATile*&       operator[](FCoordPtr index);
    ATile* const& operator[](FCoordPtr index) const;

    ATile*&       operator[](const FCoord& index);
    ATile* const& operator[](const FCoord& index) const;
    
    void Add(FCoordPtr coord, ATile* tile);
    bool Contains(FCoordPtr coord) const;
    bool Contains(const FCoord& coord) const;
    void Empty();

    int Num()      const { return map.Num(); }
    bool IsEmpty() const { return map.IsEmpty(); }

    TMap<FCoord, ATile*>::TRangedForIterator end()              { return map.end(); }
    TMap<FCoord, ATile*>::TRangedForIterator begin()            { return map.begin(); }
    TMap<FCoord, ATile*>::TRangedForConstIterator end()   const { return map.end(); }
    TMap<FCoord, ATile*>::TRangedForConstIterator begin() const { return map.begin(); }
    
    TArray<ATile*> Values() const;
private:
    TMap<FCoord, ATile*> map;
    FCoord* value = nullptr;
};