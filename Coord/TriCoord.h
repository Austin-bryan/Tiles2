#pragma once
#include "CoreMinimal.h"
#include "Enums.h"
#include "Logger.h"
#include "Coord/Coord.h"
#include "Board/Populators/TriangleBoardPopulator.h"

struct TILES2_API FTriCoord : public FCoord
{
	friend class TriangleBoardPopulator;
public:
	explicit FTriCoord(const bool isUp) : FCoord(), isUp {isUp} {}
	FTriCoord(const float x, const float y, const float z, const bool isUp);

	bool GetIsUp() const { return isUp; }

	//FCoord* operator+(const FCoord* other)  override;
	//FCoord* operator-(const FCoord* other)  override;
	FCoord* operator+(EDirection direction) override;
	//FCoord* operator-(EDirection direction) override;

	//FCoord* operator+=(const FCoord* other)  override;
	//FCoord* operator-=(const FCoord* other)  override;
	FCoord* operator+=(EDirection direction) override;
	//FCoord* operator-=(EDirection direction) override;

	FCoord* operator=(FCoord* other) override;
	FString ToString() const override
	{
		return  fstr("tri(") + FString::SanitizeFloat(X)
		 + fstr(", ") + FString::SanitizeFloat(Y)
		 + fstr(", ") + FString::SanitizeFloat(Z) + fstr(")");
	}
private:
	bool isUp;
};
int GetHalf(float n);