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
TArray<FCoordPtr> FCoord::GetAdjacent(const ABoard* board) const
{
	TArray<FCoordPtr> array;
	
	for (const auto direction : NeighborDirections[board->GetBoardShape()])
	{
		const auto neighbor = TSharedPtr<const FCoord>(*this + direction);
		if (IsAdjacent(neighbor) && board->GetTiles().Contains(neighbor))
			array.Add(neighbor);
	}
	return array;
}
bool FCoord::IsAdjacent(const FCoordPtr other) const
{
	const auto distance = FVector::Distance(FVector(x, y, z), FVector(other->x, other->y, other->z));
	return FMath::Abs(FMath::Abs(distance) - AdjacentDistance()) <= 0.4f;
}
FString FCoord::ToString() const
{
	return fstr("(")  + FString::SanitizeFloat(X()) 
		 + fstr(", ") + FString::SanitizeFloat(Y()) 
		 + fstr(", ") + FString::SanitizeFloat(Z()) + fstr(")");
}