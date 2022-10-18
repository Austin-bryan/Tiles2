#pragma once
#include "CoreMinimal.h"
#include "Enums.h"
#include "Coord/Coord.h"

struct TILES2_API FSquareCoord : public FCoord
{
public:
	FSquareCoord() : FCoord() {}
	FSquareCoord(const float x, const float z) : FCoord(x, z) { }

	FString ToString() const override;
	FCoord* operator+(EDirection direction) override;
	FCoord* operator+=(EDirection direction) override;
	FCoord* operator=(FCoord* other) override;
	//FHexCoord& operator=(const FCoord other);
	//FHexCoord& operator+=(const FCoord other);
	//FHexCoord& operator+=(EDirection dir);
};