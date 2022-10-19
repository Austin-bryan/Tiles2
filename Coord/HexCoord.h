#pragma once
#include "CoreMinimal.h"
#include "Enums.h"
#include "Coord/Coord.h"

struct TILES2_API FHexCoord : public FCoord
{
public:
	friend class TSharedPtr<const FCoord>;

	FHexCoord() : FCoord() {}
	FHexCoord(const float x, const float y, const float z) : FCoord(x, y, z) {}

	const FCoord* operator+ (EDirection direction) const override;
	const FCoord* operator+=(EDirection direction) override;
	const FCoord* operator= (const FCoord* other)  override;

	FString ToString() const override;
};