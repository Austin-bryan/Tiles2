#include "Coord/HexCoord.h"

FCoord* FHexCoord::operator+(EDirection direction)
{
	switch (direction)
	{
	case EDirection::Up:        return new FHexCoord(X + 0, Y + 1, Z - 1);
	case EDirection::Down:      return new FHexCoord(X + 0, Y - 1, Z + 1);
	case EDirection::UpRight:   return new FHexCoord(X + 1, Y + 0, Z - 1);
	case EDirection::DownLeft:  return new FHexCoord(X - 1, Y + 0, Z + 1);
	case EDirection::UpLeft:    return new FHexCoord(X - 1, Y + 1, Z + 0);
	case EDirection::DownRight: return new FHexCoord(X + 1, Y - 1, Z + 0);
	}
	return this;
}
FCoord* FHexCoord::operator=(FCoord* other)
{
	X = other->X, Y = other->Y, Z = other->Z;
	return this;
}

FString FHexCoord::ToString() const
{
	return  fstr("hex(") + FString::SanitizeFloat(X)
		+ fstr(", ") + FString::SanitizeFloat(Y)
		+ fstr(", ") + FString::SanitizeFloat(Z) + fstr(")");
}

FCoord* FHexCoord::operator+=(EDirection dir)
{
	const auto cache = static_cast<FHexCoord*>(*this + dir);
	this->X = cache->X;
	this->Y = cache->Y;
	this->Z = cache->Z;

	return this;
}
