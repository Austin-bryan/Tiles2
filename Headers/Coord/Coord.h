#pragma once
#include "CoreMinimal.h"
#include "Enums.h"
#include "ForwardDeclares.h"
#include "Coord.generated.h"

class ABoard;
/**
 * Typically stored as a TShared<const FCoord> AKA FCoordPtr
 */
USTRUCT()
struct TILES2_API FCoord
{
	GENERATED_BODY()

	FCoord();
	FCoord(
		const float,
		const float);
	FCoord(
		const float,
		const float,
		const float);
	FCoord(
		const FCoord& other);
	virtual ~FCoord() = default;
	
	static FCoordPtr Create(
		EBoardShape shape,
		const float x,
		const float z,
		const float y);
	float X() const { return x; }
	float Y() const { return y; }
	float Z() const { return z; }

	virtual FString ToString() const;
	float Largest() const;

	bool operator==(const FCoord& other) const;
	bool operator!=(const FCoord& other) const;

	virtual const FCoord* operator= (const FCoord* other) { return nullptr; }
	// TODO:: Is this used?
	virtual const FCoord* operator+=(const EDirection direction) { return nullptr; }
	
	// TODO:: Add operator- for EDirection. Will require EDirection::Opposite
	virtual const FCoord* operator+ (const FCoord* other) const;
	virtual const FCoord* operator- (const FCoord* other) const;
	virtual const FCoord* operator+ (const EDirection direction) const { return nullptr; }
	virtual const FCoord* operator- (const EDirection direction) const { return nullptr; }

	virtual float GetSpaceX()  const { return 0; }
	virtual float GetSpaceZ()  const { return 0; }
	virtual float GetOffsetX() const { return 0; }
	virtual float GetOffsetZ() const { return 0; }
	
	virtual EDirection Vertical()   const { return EDirection::Down; }
	virtual EDirection Horizontal() const { return EDirection::Right; }

	TArray<TSharedPtr<const FCoord>> GetAdjacent(const ABoard* board) const;
	bool IsAdjacent(FCoordPtr other) const;

	// ReSharper disable once CppNonExplicitConversionOperator
	operator FString() const { return ToString(); }
protected:
	float x, y, z;
	virtual float AdjacentDistance() const { return 1; }

	virtual TArray<EDirection> NeighborDirections() const
	{
		return TArray { EDirection::Up, EDirection::Down, EDirection::Right, EDirection::Left };
	}
	virtual EBoardShape GetShape() const { return EBoardShape::Square; }
};

inline uint32 GetTypeHash(
	const FCoord& coord);

inline FCoordPtr operator+ (FCoordPtr  lhs, FCoordPtr rhs);
inline FCoordPtr operator- (FCoordPtr  lhs, FCoordPtr rhs);
inline FCoordPtr operator+=(FCoordPtr& lhs, FCoordPtr rhs);
inline FCoordPtr operator-=(FCoordPtr& lhs, FCoordPtr rhs);
inline FCoordPtr operator+ (FCoordPtr  lhs, EDirection rhs);
inline FCoordPtr operator- (FCoordPtr  lhs, EDirection rhs);
inline FCoordPtr operator+=(FCoordPtr& lhs, EDirection rhs);
inline FCoordPtr operator-=(FCoordPtr& lhs, EDirection rhs);
