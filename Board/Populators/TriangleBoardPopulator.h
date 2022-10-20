#pragma once
#include "CoreMinimal.h"
#include "Board/Populators/BoardPopulator.h"
#include "ForwardDeclares.h"

class ATile;
class ABoard;

class TILES2_API TriangleBoardPopulator : public BoardPopulator
{
public:
	TriangleBoardPopulator(ABoard* const board, const EBoardShape& shape) :
		BoardPopulator(board, shape){ }
	void Populate(FCoordPtr, Tiles& tiles) override;
};