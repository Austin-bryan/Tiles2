#pragma once
#include "CoreMinimal.h"
#include "Coord.h"
#include "SqrCoord.generated.h"

enum class EDirection;

USTRUCT()
struct TILES2_API FSqrCoord : public FCoord
{
	GENERATED_BODY()
public:
	FSqrCoord() : FCoord() {}
	FSqrCoord(const float x, const float z) : FCoord(x, z) { }

	FString ToString() const override;
	const FCoord* operator+ (const EDirection direction) const override;
	const FCoord* operator+=(const EDirection direction) override;
	const FCoord* operator= (const FCoord* other) override;
	
	bool operator<(const FCoord& other) const override;

	float GetSpaceX()  const override;
	float GetSpaceZ()  const override;
	float GetOffsetX() const override;
	float GetOffsetZ() const override;
protected:
	TArray<EDirection> GetNeighborDirections() const override { return TArray { EDirection::Down, EDirection::Up, EDirection::Right, EDirection::Left }; }
};