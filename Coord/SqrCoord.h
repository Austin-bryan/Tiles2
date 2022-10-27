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
	
	bool operator<(const FCoord& other) const override
	{
		return other.X() > X() || other.Y() > Y();
	}

	float GetSpaceX()  const override;
	float GetSpaceZ()  const override;
	float GetOffsetX() const override;
	float GetOffsetZ() const override;
};

/*

-1,-1, 	0,-1,  1,-1
-1, 0, 	0, 0,  1, 0
-1, 1, 	0, 1,  1, 1

is less if (other.x > x || other.y > y);

x * y
 1     0     -1
 0     0      1 
-1     1      1

goal:
 0     1      2
 3	   4      5
 6     7      8


*/