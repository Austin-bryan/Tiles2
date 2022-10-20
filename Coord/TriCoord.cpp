#include "Coord/TriCoord.h"

int GetHalf(const float n) { return floor(n / 2); }

FTriCoord::FTriCoord(const float x, const float y, const float z, const bool isUp): FCoord(x, y, z), isUp{ isUp } {}
//TODO:: make sure this works with multiple directions in a row
const FCoord* FTriCoord::operator+(const EDirection direction) const
{
	switch (direction)
	{
	case EDirection::Left:		
		return isUp 
		? new FTriCoord(X() + 0, Y() - 1, Z() + 0, !isUp) 
		: new FTriCoord(X() + 1, Y() + 0, Z() + 0, !isUp);
	case EDirection::Right:		
		return isUp 
		? new FTriCoord(X() - 1, Y() + 0, Z() + 0, !isUp) 
		: new FTriCoord(X() + 0, Y() + 1, Z() + 0, !isUp);
	case EDirection::UpLeft:	
		return isUp 
		? new FTriCoord(X() + 0, Y() - 1, Z() + 0, !isUp) 
		: new FTriCoord(X() + 0, Y() + 0, Z() - 1, !isUp);
	case EDirection::UpRight:	
		return isUp 
		? new FTriCoord(X() - 1, Y() + 0, Z() + 0, !isUp) 
		: new FTriCoord(X() + 0, Y() + 0, Z() - 1, !isUp);
	case EDirection::DownLeft:	
		return isUp 
		? new FTriCoord(X() + 0, Y() + 0, Z() + 1, !isUp) 
		: new FTriCoord(X() + 1, Y() + 0, Z() + 0, !isUp);
	case EDirection::DownRight:	
		return isUp 
		? new FTriCoord(X() + 0, Y() + 0, Z() + 1, !isUp) 
		: new FTriCoord(X() + 0, Y() + 1, Z() + 0, !isUp);
	default: return this;
	}
}
const FCoord* FTriCoord::operator=(const FCoord* other)
{
	x = other->X(), y = other->Y(), z = other->Z();
	isUp = static_cast<const FTriCoord*>(other)->isUp;

	return this;
}

float FTriCoord::GetSpaceX()  const { return 67; }
float FTriCoord::GetSpaceZ()  const { return 120; }
float FTriCoord::GetOffsetX() const { return -x + y; }
float FTriCoord::GetOffsetZ() const { return -z; }

const FCoord* FTriCoord::operator+=(const EDirection direction)
{
	const auto cache = static_cast<const FTriCoord*>(*this + direction);
	x = cache->X();
	y = cache->Y();
	z = cache->Z();
	this->isUp  = cache->isUp;

	return this;
}