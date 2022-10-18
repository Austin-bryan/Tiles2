#pragma once
#include "CoreMinimal.h"

enum class EBoardShape;
class ABoard;
class ATile;
struct FCoord;

/**
 *	Populates the board differently depending on the board shape- 
 */
class TILES2_API BoardPopulator
{
public:
	virtual ~BoardPopulator() = default;
	BoardPopulator(ABoard* const, const EBoardShape&);
	virtual void Populate(FCoord*, TMap<FCoord*, ATile*>& tiles) {}
protected:
	ABoard* board;
	EBoardShape boardShape;

	virtual float GetSpaceX() const = 0;
	virtual float GetSpaceZ() const = 0;
	virtual float GetOffsetX(FCoord*) const = 0;
	virtual float GetOffsetZ(FCoord*) const = 0;

	ATile* CreateTile(FCoord*, TMap<FCoord*, ATile*>& tiles) const;
};