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
	
	float GetSpaceX() const override { return 110; }
	float GetSpaceZ() const override { return 110; }
	float GetOffsetX(FCoordPtr coord) const override;
	float GetOffsetZ(FCoordPtr coord) const override;
};
