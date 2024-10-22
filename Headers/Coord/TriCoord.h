#pragma once
#include "CoreMinimal.h"
#include "Coord.h"
#include "TriCoord.generated.h"

enum class EDirection;
class TriBoardPopulator;

USTRUCT()
struct TILES2_API FTriCoord : public FCoord
{
	GENERATED_BODY()
	friend class TriBoardPopulator;
public:
	FTriCoord();
	explicit FTriCoord(const bool isUp);
	FTriCoord(const float x, const float y, const float z, const bool isUp);
	bool GetIsUp() const { return isUp; }

	const FCoord* operator+ (EDirection direction) const override;
	const FCoord* operator+=(EDirection direction) override;
	const FCoord* operator= (const FCoord* other) override;
	const FCoord* operator+ (const FCoord* other) const override;	
	const FCoord* operator- (const FCoord* other) const override;	

	FString ToString() const override;
	float GetSpaceX()  const override;
	float GetSpaceZ()  const override;
	float GetOffsetX() const override;
	float GetOffsetZ() const override;
	
	TArray<EDirection> NeighborDirections() const override
	{
		return isUp ? TArray{ EDirection::Left, EDirection::Right, EDirection::DownLeft }
					: TArray{ EDirection::Left, EDirection::Right, EDirection::UpRight }; 
	}
protected:
	EBoardShape GetShape() const override { return EBoardShape::Triangle; }
private:
	bool isUp;
};
int GetHalf(const float n);