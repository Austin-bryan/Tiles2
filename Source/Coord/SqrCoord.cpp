#include "SqrCoord.h"
#include "Logger.h"
#include "Enums.h"

const FCoord* FSqrCoord::operator+(const EDirection direction) const
{
	switch (direction)
	{
	case EDirection::Up:    return new FSqrCoord(x + 0, z - 1);
	case EDirection::Down:  return new FSqrCoord(x + 0, z + 1);
	case EDirection::Left:  return new FSqrCoord(x - 1, z + 0);
	case EDirection::Right: return new FSqrCoord(x + 1, z + 0);
	default: return this;
	}
}

float FSqrCoord::GetSpaceX()  const { return 102.5f; }
float FSqrCoord::GetSpaceZ()  const { return 102.5f; }
float FSqrCoord::GetOffsetX() const { return -x; }
float FSqrCoord::GetOffsetZ() const { return -z; }

const FCoord* FSqrCoord::operator=(const FCoord* other)
{
	x = other->X(), z = other->Z();
	return this;
}

const FCoord* FSqrCoord::operator+=(const EDirection direction)
{
	const auto cache = static_cast<const FSqrCoord*>(*this + direction);
	this->x = cache->x;
	this->z = cache->z;

	return this;
}
FString FSqrCoord::ToString() const
{
	return fstr("sqr(") + FString::SanitizeFloat(X())
		 + fstr(", ")   + FString::SanitizeFloat(Z()) + fstr(")");
}