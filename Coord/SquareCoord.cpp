#include "Coord/SquareCoord.h"
#include "Logger.h"

const FCoord* FSquareCoord::operator+(const EDirection direction) const
{
	switch (direction)
	{
	case EDirection::Up:    return new FSquareCoord(x + 0, z - 1);
	case EDirection::Down:  return new FSquareCoord(x + 0, z + 1);
	case EDirection::Left:  return new FSquareCoord(x - 1, z + 0);
	case EDirection::Right: return new FSquareCoord(x + 1, z + 0);
	default: return this;
	}
}

float FSquareCoord::GetSpaceX()  const { return 110; }
float FSquareCoord::GetSpaceZ()  const { return 110; }
float FSquareCoord::GetOffsetX() const { return -x; }
float FSquareCoord::GetOffsetZ() const { return -z; }

const FCoord* FSquareCoord::operator=(const FCoord* other)
{
	x = other->X(), z = other->Z();
	return this;
}

const FCoord* FSquareCoord::operator+=(const EDirection direction)
{
	const auto cache = static_cast<const FSquareCoord*>(*this + direction);
	this->x = cache->x;
	this->z = cache->z;

	return this;
}
FString FSquareCoord::ToString() const
{
	return fstr("sqr(") + FString::SanitizeFloat(X())
		 + fstr(", ")   + FString::SanitizeFloat(Z()) + fstr(")");
}