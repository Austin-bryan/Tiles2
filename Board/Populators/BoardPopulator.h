#pragma once
#include "CoreMinimal.h"
#include "ForwardDeclares.h"

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
	virtual void Populate(FCoordPtr, Tiles& tiles) {}
protected:
	ABoard* board;
	EBoardShape boardShape;
	ATile* CreateTile(FCoordPtr, Tiles& tiles) const;
};