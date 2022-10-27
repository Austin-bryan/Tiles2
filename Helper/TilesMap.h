#pragma once
#include "CoreMinimal.h"
#include "ForwardDeclares.h"

struct FCoord;

/**
 * Uses FCoord as the key as opposed to FCoordPtr for ease of use.
 * Maintains original FCoordPtr's incase GetKeys() are called, to get the original unsliced version
 */
class Tiles
{
public:
    ATile*& operator[](FCoordPtr index);
    ATile* const& operator[](FCoordPtr index) const;
    
    void Add(FCoordPtr coord, ATile* tile);
    bool Contains(FCoordPtr coord) const;
    
    TArray<FCoordPtr> Keys() const;
    TArray<ATile*> Values() const;
private:
    TMap<CoordMembers, ATile*> map;
    TMap<CoordMembers, FCoordPtr> sliceMap;
};