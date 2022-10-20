#pragma once
#include "CoreMinimal.h"
#include "Board/Populators/BoardPopulator.h"
#include "ForwardDeclares.h"

struct FCoord;
class ABoard;

class TILES2_API SquareBoardPopulator final : public BoardPopulator
{
public:
	SquareBoardPopulator(ABoard* const board, const EBoardShape& shape) :
		BoardPopulator(board, shape){ }
	void Populate(FCoordPtr size, Tiles& tiles) override;
};
