#pragma once
#include "CoreMinimal.h"
#include "Enums.h"
#include "Coord/Coord.h"

struct TILES2_API FHexCoord : public FCoord
{
public:
	FHexCoord() : FCoord() {}
	FHexCoord(const float x, const float y, const float z) : FCoord(x, y, z) {}

	FCoord* operator+(EDirection direction) override;
	FCoord* operator+=(EDirection direction) override;
	FCoord* operator=(FCoord* other) override;
	//FHexCoord& operator=(const FCoord other);
	//FHexCoord& operator+=(const FCoord other);
	//FHexCoord& operator+=(EDirection dir);

	FString ToString() const override;
};