#pragma once
#include "CoreMinimal.h"
#include "Coord/Coord.h"

enum class EDirection;

struct TILES2_API FSqrCoord : public FCoord
{
public:
	FSqrCoord() : FCoord() {}
	FSqrCoord(const float x, const float z) : FCoord(x, z) { }

	FString ToString() const override;
	const FCoord* operator+ (const EDirection direction) const override;
	const FCoord* operator+=(const EDirection direction) override;
	const FCoord* operator= (const FCoord* other) override;

	float GetSpaceX()  const override;
	float GetSpaceZ()  const override;
	float GetOffsetX() const override;
	float GetOffsetZ() const override;
};