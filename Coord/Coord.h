#pragma once
#include "CoreMinimal.h"
#include "Enums.h"
#include "ForwardDeclares.h"

//todo:: have operators for fcoord be friend shared ptr, shared ptr and return shared ptr
struct TILES2_API FCoord
{
	static FCoordPtr Create(EBoardShape shape, const float x, const float z, const float y);

	FCoord();
	FCoord(const float, const float);
	FCoord(const float, const float, const float);
	virtual ~FCoord() = default;
	
	float X() const { return x; }
	float Y() const { return y; }
	float Z() const { return z; }

	virtual FString ToString() const;
	float Largest() const;

	bool operator==(const FCoord& other) const { return x == other.x && y == other.y && z == other.z; }
	bool operator!=(const FCoord& other) const { return !(*this == other); }

	virtual const FCoord* operator+ (const EDirection direction) const = 0;
	virtual const FCoord* operator= (const FCoord* other)		       = 0;
	virtual const FCoord* operator+=(const EDirection direction)       = 0;

	virtual float GetSpaceX()  const = 0;
	virtual float GetSpaceZ()  const = 0;
	virtual float GetOffsetX() const = 0;
	virtual float GetOffsetZ() const = 0;

	// ReSharper disable once CppNonExplicitConversionOperator
	operator FString() const { return ToString(); }

protected:
	float x, y, z;
};