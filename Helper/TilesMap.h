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
    
    int Num()      const { return map.Num(); }
    bool IsEmpty() const { return map.IsEmpty(); }

    TArray<ATile*> Values() const;
    // TArray<FCoordPtr> Keys() const;
private:
    TArray<TArray<TArray<ATile*>>> map;

    USTRUCT()
    struct FInner
    {
        TArray<ATile*> Array;
        ATile* operator[]a      
    };
};