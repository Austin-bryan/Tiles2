#pragma once
#include "CoreMinimal.h"
#include "Enums.h"
#include "ForwardDeclares.h"
#include "Coord.generated.h"

//todo:: have operators for fcoord be friend shared ptr, shared ptr and return shared ptr
/**
 * Typically stored as a TShared<const FCoord> AKA FCoordPtr
 */
USTRUCT()
struct TILES2_API FCoord
{
	//todo:: add hashings
	GENERATED_BODY()

	FCoord();
	FCoord(const float, const float);
	FCoord(const float, const float, const float);
	FCoord(const FCoord& other);
	virtual ~FCoord() = default;
	
	static FCoordPtr Create(EBoardShape shape, const float x, const float z, const float y);
	float X() const { return x; }
	float Y() const { return y; }
	float Z() const { return z; }

	virtual FString ToString() const;
	float Largest() const;

	bool operator==(const FCoord& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}
	bool operator!=(const FCoord& other) const { return !(*this == other); }

	//todo:: remove
	virtual bool operator<(const FCoord& other) const { return false; }

	static FCoordPtr Distance(const FCoordPtr a, const FCoordPtr b);

	virtual const FCoord* operator+ (const EDirection direction) const { return nullptr; }
	virtual const FCoord* operator= (const FCoord* other)		       { return nullptr; }
	virtual const FCoord* operator+=(const EDirection direction)       { return nullptr; }

	virtual float GetSpaceX()  const { return 0; }
	virtual float GetSpaceZ()  const { return 0; }
	virtual float GetOffsetX() const { return 0; }
	virtual float GetOffsetZ() const { return 0; }

	// ReSharper disable once CppNonExplicitConversionOperator
	operator FString() const { return ToString(); }
protected:
	float x, y, z;
};

inline uint32 GetTypeHash(const FCoord& coord)
{
	return
		 (std::hash<float>()(coord.X())
		^ std::hash<float>()(coord.Y()) << 1) >> 1
		^ std::hash<float>()(coord.Z()) << 1;
}	
inline FCoordPtr operator+(const FCoordPtr coord, const EDirection direction)
{
	return FCoordPtr(*coord + direction);
}
inline FCoordPtr operator+=(FCoordPtr& coord, const EDirection direction)
{
	return coord = coord + direction;
}