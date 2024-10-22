#include "HexCoord.h"
#include "Logger.h"

const FCoord* FHexCoord::operator+ (const EDirection direction) const
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
const FCoord* FHexCoord::operator= (const FCoord* other)
{
	x = other->X(), y = other->Y(), z = other->Z();
	return this;
}
const FCoord* FHexCoord::operator+=(const EDirection direction)
{
	const auto cache = TUniquePtr<const FCoord>(*this + direction);
	x = cache->X();
	y = cache->Y();
	z = cache->Z();

	return this;
}
const FCoord* FHexCoord::operator+ (const FCoord* other) const { return new FHexCoord(x + other->X(), y + other->Y(), z + other->Z()); }
const FCoord* FHexCoord::operator- (const FCoord* other) const { return new FHexCoord(x - other->X(), y - other->Y(), z - other->Z()); }

const float spaceModifier = 1.0f;
float FHexCoord::GetSpaceX()  const { return 45 * spaceModifier; }
float FHexCoord::GetSpaceZ()  const { return 52.0f * spaceModifier; }
float FHexCoord::GetOffsetX() const { return -x + y + z; }
float FHexCoord::GetOffsetZ() const { return -z + y; }
FString FHexCoord::ToString() const { return "hex("_f + X() + ", "_f + Y() + ", "_f + Z() + ")"_f; }