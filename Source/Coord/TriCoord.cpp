#include "TriCoord.h"

#include "CreatorBoard.h"
#include "Logger.h"
#include "Enums.h"

int GetHalf(const float n) { return floor(n / 2); }

FTriCoord::FTriCoord() : isUp{ false } { }
FTriCoord::FTriCoord(const bool isUp): FCoord(), isUp {isUp} { }
FTriCoord::FTriCoord(const float x, const float y, const float z, const bool isUp): FCoord(x, y, z), isUp{ isUp } {}

const FCoord* FTriCoord::operator+(const EDirection direction) const
{
	switch (direction)
	{
	case EDirection::Left:      return isUp ? new FTriCoord(X(), Y() - 1, Z(), !isUp) : new FTriCoord(X() + 1, Y(), Z(), !isUp);
	case EDirection::Right:     return isUp ? new FTriCoord(X() - 1, Y(), Z(), !isUp) : new FTriCoord(X(), Y() + 1, Z(), !isUp);
	case EDirection::UpLeft:    return isUp ? new FTriCoord(X(), Y() - 1, Z(), !isUp) : new FTriCoord(X(), Y(), Z() - 1, !isUp);
	case EDirection::UpRight:   return isUp ? new FTriCoord(X() - 1, Y(), Z(), !isUp) : new FTriCoord(X(), Y(), Z() - 1, !isUp);
	case EDirection::DownLeft:  return isUp ? new FTriCoord(X(), Y(), Z() + 1, !isUp) : new FTriCoord(X() + 1, Y(), Z(), !isUp);
	case EDirection::DownRight:	return isUp ? new FTriCoord(X(), Y(), Z() + 1, !isUp) : new FTriCoord(X(), Y() + 1, Z(), !isUp);
	case EDirection::Up:		return isUp ? new FTriCoord(X(), Y(), Z() + 1, !isUp) : new FTriCoord(X(), Y(), Z() - 1, !isUp);
	default: return this;
	}
}
const FCoord* FTriCoord::operator=(const FCoord* other)
{
	x = other->X(), y = other->Y(), z = other->Z();
	isUp = static_cast<const FTriCoord*>(other)->isUp;
	return this;
}
const FCoord* FTriCoord::operator+(const FCoord* other) const
{
	auto result = FVector(other->X() + x, other->Y() + y, other->Z() + z);
	auto sum = result.X + result.Y + result.Z;
	auto isEven = static_cast<int>(sum) % 2 == 0;

	// TODO:: Make sure this works
	// TODO:: refactor operator+- to use same code base
	return new FTriCoord(other->X() + x, other->Y() + y, other->Z() + z, isEven ? isUp : !isUp);
} 
const FCoord* FTriCoord::operator-(const FCoord* other) const
{
	auto result = FVector(other->X() - x, other->Y() - y, other->Z() - z);
	auto sum = result.X + result.Y + result.Z;
	auto isEven = static_cast<int>(sum) % 2 == 0;

	return new FTriCoord(result.X, result.Y, result.Z, isEven ? isUp : !isUp);
}

FString FTriCoord::ToString() const { return "tri("_f + X() + ", "_f + Y() + ", "_f + Z() + ")"_f; }

const float triSpacing = 1.0f;
// float FTriCoord::GetSpaceX()  const { return 71 * triSpacing; }
float FTriCoord::GetSpaceX()  const { return 67 * triSpacing; }
float FTriCoord::GetSpaceZ()  const { return 115.5f * triSpacing; }
float FTriCoord::GetOffsetX() const { return -x + y; }
float FTriCoord::GetOffsetZ() const { return -z; }

const FCoord* FTriCoord::operator+=(const EDirection direction)
{
	const auto cache = static_cast<const FTriCoord*>(*this + direction);
	x = cache->X();
	y = cache->Y();
	z = cache->Z();
	this->isUp = cache->isUp;

	return this;
}