#include "Coord/Coord.h"
#include "SquareCoord.h"
#include "TriCoord.h"
#include "HexCoord.h"

FCoord* FCoord::Create(EBoardShape shape, int X, int Z, int Y)
{
	switch (shape)
	{
	case EBoardShape::Triangle: return new FTriCoord(X, Y, Z, true);
	case EBoardShape::Hex:      return new FHexCoord(X, Y, Z);
	default:					return new FSquareCoord(X, Z);
	}
}

FCoord::FCoord()						  : X{ 0 }, Y{ 0 }, Z{ 0 } {}
FCoord::FCoord(float x, float z)		  : X{ x }, Y{ 0 }, Z{ z } {}
FCoord::FCoord(float x, float y, float z) : X{ x }, Y{ y }, Z{ z } {}

float FCoord::Largest() const
{
	return X > Y
		? X > Z ? X : Z
		: Y > Z ? Y : Z;
}
FString FCoord::ToString() const
{
	return fstr("(")  + FString::SanitizeFloat(X) 
		 + fstr(", ") + FString::SanitizeFloat(Y) 
		 + fstr(", ") + FString::SanitizeFloat(Z) + fstr(")");
}