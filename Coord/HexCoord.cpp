#include "Coord/HexCoord.h"
#include "Logger.h"

const FCoord* FHexCoord::operator+(const EDirection direction) const
{
	switch (direction)
	{
	case EDirection::Up:        return new FHexCoord(x + 0, y + 1, z - 1);
	case EDirection::Down:      return new FHexCoord(x + 0, y - 1, z + 1);
	case EDirection::UpRight:   return new FHexCoord(x + 1, y + 0, z - 1);
	case EDirection::DownLeft:  return new FHexCoord(x - 1, y + 0, z + 1);
	case EDirection::UpLeft:    return new FHexCoord(x - 1, y + 1, z + 0);
	case EDirection::DownRight: return new FHexCoord(x + 1, y - 1, z + 0);
	default: throw std::invalid_argument("Invalid direction enum");
	}
}
const FCoord* FHexCoord::operator=(const FCoord* other)
{
	x = other->X(), y = other->Y(), z = other->Z();
	return this;
}
const FCoord* FHexCoord::operator+=(const EDirection direction)
{
	const auto cache = static_cast<const FHexCoord*>(*this + direction);
	x = cache->x;
	y = cache->y;
	z = cache->z;

	return this;
}
FString FHexCoord::ToString() const
{
	return fstr("hex(") + FString::SanitizeFloat(X())
		 + fstr(", ")   + FString::SanitizeFloat(Y())
		 + fstr(", ")   + FString::SanitizeFloat(Z()) + fstr(")");
}
