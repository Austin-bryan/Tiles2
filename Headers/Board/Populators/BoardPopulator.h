#pragma once
#include "CoreMinimal.h"
#include "ForwardDeclares.h"
#include "TilesMap.h"

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
	explicit BoardPopulator(ABoard* const);
	virtual void Populate(FCoordPtr, TilesMap& tiles) {}
protected:
	ABoard* board;
	EBoardShape boardShape;
	ATile* CreateTile(FCoordPtr, TilesMap& tiles) const;
};