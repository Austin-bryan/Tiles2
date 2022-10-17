#include "Coord/SquareCoord.h"

FCoord* FSquareCoord::operator+(EDirection direction)
{
	switch (direction)
	{
	case EDirection::Up:    return new FSquareCoord(X + 0, Z - 1);
	case EDirection::Down:  return new FSquareCoord(X + 0, Z + 1);
	case EDirection::Left:  return new FSquareCoord(X - 1, Z + 0);
	case EDirection::Right: return new FSquareCoord(X + 1, Z + 0);
	}
	return this;
}
FCoord* FSquareCoord::operator=(FCoord* other)
{
	X = other->X, Z = other->Z;
	return this;
}
FCoord* FSquareCoord::operator+=(EDirection dir)
{
	const auto cache = static_cast<FSquareCoord*>(*this + dir);
	this->X = cache->X;
	this->Z = cache->Z;

	return this;
}
FString FSquareCoord::ToString() const
{
	return fstr("sqr(") + FString::SanitizeFloat(X)
		 + fstr(", ") + FString::SanitizeFloat(Z) + fstr(")");
}