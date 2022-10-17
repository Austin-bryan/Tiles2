#pragma once
#include "CoreMinimal.h"
#include "Board/Populators/BoardPopulator.h"

struct FCoord;
class ABoard;

class TILES2_API SquareBoardPopulator final : public BoardPopulator
{
public:
	SquareBoardPopulator(ABoard* const board, const EBoardShape& shape) :
		BoardPopulator(board, shape){ }
	void Populate(FCoord* size, TMap<FCoord*, ATile*>& tiles) override;
	
	float GetSpaceX() const override { return 110; }
	float GetSpaceZ() const override { return 110; }
	float GetOffsetX(FCoord* coord) const override;
	float GetOffsetZ(FCoord* coord) const override;
};
