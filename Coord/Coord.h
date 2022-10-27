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

	//todo:: impliment ineqluaities for all coords
	virtual bool operator<(const FCoord& other) const { return false; }

	virtual const FCoord* operator+ (const EDirection direction) const = 0;
	virtual const FCoord* operator= (const FCoord* other)		       = 0;
	virtual const FCoord* operator+=(const EDirection direction)       = 0;

	virtual float GetSpaceX()  const = 0;
	virtual float GetSpaceZ()  const = 0;
	virtual float GetOffsetX() const = 0;
	virtual float GetOffsetZ() const = 0;

	static CoordMembers GetMembers(const FCoordPtr coord)
	{
		return CoordMembers(coord->X(), coord->Y(), coord->Z());
	}

	// ReSharper disable once CppNonExplicitConversionOperator
	operator FString() const { return ToString(); }

protected:
	float x, y, z;
};



// #include <map>
// template<>
// struct std::less<FSqrCoordPtr>
// {
// 	bool operator() (const FCoordPtr& lhs, const FCoordPtr& rhs)
// 	{
// 		// TMap
// 		return *lhs < *rhs;
// 	}
// };
 
// inline bool operator ==(const FCoordPtr lhs, const FCoordPtr rhs)
// {
// 	return *lhs == *rhs;
// }
// inline bool operator !=(const FCoordPtr lhs, const FCoordPtr rhs)
// {
// 	return *lhs != *rhs;
// }
inline FCoordPtr operator+(const FCoordPtr coord, const  EDirection direction)
{
	return FCoordPtr(*coord + direction);
}
inline FCoordPtr operator+=(FCoordPtr& coord, const EDirection direction)
{
	return coord = coord + direction;
}