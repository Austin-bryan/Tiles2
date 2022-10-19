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
	const FCoord* operator+ (const EDirection direction) const override;
	const FCoord* operator+=(const EDirection direction) override;
	const FCoord* operator= (const FCoord* other) override;
};