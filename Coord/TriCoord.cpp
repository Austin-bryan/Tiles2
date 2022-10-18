#include "Coord/TriCoord.h"
#include "Board/BoardCordinator.h"
#include "Logger.h"

int GetHalf(const float n) { return floor(n / 2); }

FTriCoord::FTriCoord(const float x, const float y, const float z, const bool isUp): FCoord(x, y, z), isUp{ isUp } {}
//TODO:: make sure this works with multiple directions in a row
FCoord* FTriCoord::operator+(const EDirection direction)
{
	switch (direction)
	{
	case EDirection::Left:		return isUp ? new FTriCoord(X + 0, Y - 1, Z + 0, !isUp) : new FTriCoord(X + 1, Y + 0, Z + 0, !isUp);
	case EDirection::Right:		return isUp ? new FTriCoord(X - 1, Y + 0, Z + 0, !isUp) : new FTriCoord(X + 0, Y + 1, Z + 0, !isUp);
	case EDirection::UpLeft:	return isUp ? new FTriCoord(X + 0, Y - 1, Z + 0, !isUp) : new FTriCoord(X + 0, Y + 0, Z - 1, !isUp);
	case EDirection::UpRight:	return isUp ? new FTriCoord(X - 1, Y + 0, Z + 0, !isUp) : new FTriCoord(X + 0, Y + 0, Z - 1, !isUp);
	case EDirection::DownLeft:	return isUp ? new FTriCoord(X + 0, Y + 0, Z + 1, !isUp) : new FTriCoord(X + 1, Y + 0, Z + 0, !isUp);
	case EDirection::DownRight:	return isUp ? new FTriCoord(X + 0, Y + 0, Z + 1, !isUp) : new FTriCoord(X + 0, Y + 1, Z + 0, !isUp);
	default: return this;
	}
}
FCoord* FTriCoord::operator=(FCoord* other)
{
	X = other->X, Y = other->Y, Z = other->Z;
	isUp = static_cast<FTriCoord*>(other)->isUp;

	return this;
}
FCoord* FTriCoord::operator+=(const EDirection direction)
{
	const auto cache = static_cast<FTriCoord*>(*this + direction);
	this->X		= cache->X;
	this->Y		= cache->Y;
	this->Z		= cache->Z;
	this->isUp  = cache->isUp;

	return this;
}