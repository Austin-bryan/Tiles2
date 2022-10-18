#pragma once
#include "CoreMinimal.h"
#include "Board/Populators/BoardPopulator.h"

class ATile;
class ABoard;

class TILES2_API TriangleBoardPopulator : public BoardPopulator
{
public:
	TriangleBoardPopulator(ABoard* const board, const EBoardShape& shape) :
		BoardPopulator(board, shape){ }
	void Populate(FCoord*, TMap<FCoord*, ATile*>& tiles) override;

	float GetSpaceX() const override { return 67; }
	float GetSpaceZ() const override { return 120; }
	float GetOffsetX(FCoord* coord) const override;
	float GetOffsetZ(FCoord* coord) const override;
};