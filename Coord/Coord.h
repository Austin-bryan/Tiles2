#pragma once
#include "CoreMinimal.h"
#include "Enums.h"
#include "Logger.h"
#include <string>
//#include "Coord.generated.h"

//USTRUCT(BlueprintType)
struct TILES2_API FCoord
{
//	GENERATED_BODY();
public:
virtual ~FCoord() = default;
static FCoord* Create(EBoardShape shape, int X, int Z, int Y);

	FCoord();
	FCoord(float, float);
	FCoord(float, float, float);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Z;

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

	operator FString() const { return ToString(); }
};