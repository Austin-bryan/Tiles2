#pragma once
#include "CoreMinimal.h"
#include "Enums.h"
#include <string>

struct TILES2_API FCoord
{
virtual ~FCoord() = default;
static FCoord* Create(EBoardShape shape, int X, int Z, int Y);

	FCoord();
	FCoord(const float, const float);
	FCoord(const float, const float, const float);

	float X, Y, Z;

	virtual FString ToString() const;
	float Largest() const;

	bool operator==(const FCoord& other) const { return X == other.X && Y == other.Y && Z == other.Z; }
	bool operator!=(const FCoord& other) const { return !(*this == other); }

	//virtual FCoord* operator+(const FCoord* other)  = 0;
	//virtual FCoord* operator-(const FCoord* other)  = 0;
	virtual FCoord* operator+(EDirection direction) { return new FCoord(0,0,0); }
	//virtual FCoord* operator-(EDirection direction) = 0;

	//virtual FCoord* operator+=(const FCoord* other) = 0;
	//virtual FCoord* operator-=(const FCoord* other) = 0;
	virtual FCoord* operator+=(EDirection direction) { return new FCoord(0, 0, 0); }
	//virtual FCoord* operator-=(EDirection direction) = 0;

	virtual FCoord* operator=(FCoord* other) { return new FCoord(0, 0, 0); }
	//FCoord operator-(const FCoord& other) const { return FCoord(X - other.X, Y - other.Y); }
	//FCoord operator*(const FCoord& other) const { return FCoord(X * other.X, Y * other.Y, Z * other.Z); }
	//FCoord operator/(const FCoord& other) const { return FCoord(X / other.X, Y / other.Y, Z / other.Z); }

	// ReSharper disable once CppNonExplicitConversionOperator
	operator FString() const { return ToString(); }
};