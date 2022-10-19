#include "Coord/Coord.h"
#include "ForwardDeclares.h"
#include "SquareCoord.h"
#include "TriCoord.h"
#include "HexCoord.h"

FCoordPtr FCoord::Create(const EBoardShape shape, const float x, const float z, const float y)
{
	switch (shape)
	{
	case EBoardShape::Triangle: return MakeShared<const FTriCoord>   (x, y, z, true);
	case EBoardShape::Hex:      return MakeShared<const FHexCoord>   (x, y, z);
	default:					return MakeShared<const FSquareCoord>(x, z);
	}
}
FCoord::FCoord()											: x{ 0 }, y{ 0 }, z{ 0 } {}
FCoord::FCoord(const float x, const float z)				: x{ x }, y{ 0 }, z{ z } {}
FCoord::FCoord(const float x, const float y, const float z) : x{ x }, y{ y }, z{ z } {}

float FCoord::Largest() const
{
	return x > y
		? x > z ? x : z
		: y > z ? y : z;
}
FString FCoord::ToString() const
{
	return fstr("(")  + FString::SanitizeFloat(X()) 
		 + fstr(", ") + FString::SanitizeFloat(Y()) 
		 + fstr(", ") + FString::SanitizeFloat(Z()) + fstr(")");
}