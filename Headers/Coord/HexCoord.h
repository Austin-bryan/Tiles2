#pragma once
#include "CoreMinimal.h"
#include "Enums.h"
#include "Coord.h"
#include "HexCoord.generated.h"

USTRUCT()
struct TILES2_API FHexCoord : public FCoord
{
	GENERATED_BODY()
public:
	friend class TSharedPtr<const FCoord>;

	FHexCoord() : FCoord() {}
	FHexCoord(const float x, const float y, const float z) : FCoord(x, y, z) {}

	const FCoord* operator+ (EDirection direction) const override;
	const FCoord* operator+=(EDirection direction) override;
	const FCoord* operator= (const FCoord* other) override;
	const FCoord* operator+ (const FCoord* other) const override;	
	const FCoord* operator- (const FCoord* other) const override;
	
	float GetSpaceX()  const override;
	float GetSpaceZ()  const override;
	float GetOffsetX() const override;
	float GetOffsetZ() const override;

	FString ToString() const override;
	float AdjacentDistance() const override { return FMath::Sqrt(2.0f); }

	TArray<EDirection> NeighborDirections() const override
	{
		return { EDirection::Up,   EDirection::Down,  EDirection::UpRight, EDirection::UpLeft, EDirection::DownRight, EDirection::DownLeft } ;
	}
protected:
	EBoardShape GetShape() const override { return EBoardShape::Hex; }
};