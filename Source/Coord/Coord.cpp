#include "Coord.h"
#include "SqrCoord.h"
#include "TriCoord.h"
#include "HexCoord.h"
#include "ForwardDeclares.h"
#include "Logger.h"
#include "Board.h"

FCoordPtr FCoord::Create(const EBoardShape shape, const float x, const float z, const float y)
{
	switch (shape)
	{
	case EBoardShape::Triangle: return MakeShared<const FTriCoord>(x, y, z, true);
	case EBoardShape::Hex:      return MakeShared<const FHexCoord>(x, y, z);
	default:					return MakeShared<const FSqrCoord>(x, z);
	}
}
FCoord::FCoord()											: x{ 0 }, y{ 0 }, z{ 0 } {}
FCoord::FCoord(const float x, const float z)				: x{ x }, y{ 0 }, z{ z } {}
FCoord::FCoord(const float x, const float y, const float z) : x{ x }, y{ y }, z{ z } {}
FCoord::FCoord(const FCoord& other)							: x{ other.x }, y{ other.y }, z{ other.z } { }

float FCoord::Largest() const { return x > y
		? x > z ? x : z
		: y > z ? y : z; }
bool FCoord::operator==(const FCoord& other) const { return x == other.x && y == other.y && z == other.z; }
bool FCoord::operator!=(const FCoord& other) const { return !(*this == other); }

const FCoord* FCoord::operator+(const FCoord* other) const { return new FCoord(x + other->x, y + other->y, z + other->z); }
const FCoord* FCoord::operator-(const FCoord* other) const { return new FCoord(x - other->x, y - other->y, z - other->z); }

TArray<FCoordPtr> FCoord::GetAdjacent(const ABoard* board) const
{
	TArray<FCoordPtr> array;

	for (const auto direction : NeighborDirections())
	{
		const auto neighbor = TSharedPtr<const FCoord>(*this + direction);
		
		// Log(ToString() + ": "_f + IsAdjacent(neighbor), board->GetTiles().Contains(neighbor));
		if (IsAdjacent(neighbor) && board->GetTiles().Contains(neighbor))
		{
			// Log(ToString() + ": "_f + neighbor->ToString(), BLUE);
			array.Add(neighbor);
		}
	}
	return array;
}
bool FCoord::IsAdjacent(const FCoordPtr other) const
{
	const auto distance = FVector::Distance(FVector(x, y, z), FVector(other->x, other->y, other->z));
	return FMath::Abs(FMath::Abs(distance) - AdjacentDistance()) <= 0.4f;
}
uint32 GetTypeHash(const FCoord& coord)
{
	return (std::hash<float>()(coord.X())
	      ^ std::hash<float>()(coord.Y()) << 1) >> 1
	      ^ std::hash<float>()(coord.Z()) << 1;
}

FCoordPtr operator+ (FCoordPtr  lhs, FCoordPtr rhs)  { return 		FCoordPtr(*lhs + rhs.Get()); }
FCoordPtr operator- (FCoordPtr  lhs, FCoordPtr rhs)  { return 		FCoordPtr(*lhs - rhs.Get()); }
FCoordPtr operator+=(FCoordPtr& lhs, FCoordPtr rhs)  { return lhs = FCoordPtr(*lhs + rhs.Get()); }
FCoordPtr operator-=(FCoordPtr& lhs, FCoordPtr rhs)  { return lhs = FCoordPtr(*lhs - rhs.Get()); }
FCoordPtr operator+ (FCoordPtr  lhs, EDirection rhs) { return 		FCoordPtr(*lhs + rhs); }
FCoordPtr operator- (FCoordPtr  lhs, EDirection rhs) { return 		FCoordPtr(*lhs - rhs); }
FCoordPtr operator+=(FCoordPtr& lhs, EDirection rhs) { return lhs = FCoordPtr(*lhs + rhs); }
FCoordPtr operator-=(FCoordPtr& lhs, EDirection rhs) { return lhs = FCoordPtr(*lhs - rhs); }

FString FCoord::ToString() const
{
	return fstr("(")  + FString::SanitizeFloat(X()) 
		 + fstr(", ") + FString::SanitizeFloat(Y()) 
		 + fstr(", ") + FString::SanitizeFloat(Z()) + fstr(")");
}
