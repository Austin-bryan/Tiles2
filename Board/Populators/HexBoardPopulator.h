#pragma once
#include "CoreMinimal.h"
#include "Board/Populators/BoardPopulator.h"

struct FCoord;
enum class EBoardShape;

class TILES2_API HexBoardPopulator final : public BoardPopulator
{
public:
	HexBoardPopulator(ABoard* const board, const EBoardShape& shape) :
		BoardPopulator(board, shape){ }
	void Populate(FCoord*, TMap<FCoord*, ATile*>& tiles) override;

	float GetSpaceX() const override { return 45; }
	float GetSpaceZ() const override { return 50; }
	float GetOffsetX(FCoord* coord) const override;
	float GetOffsetZ(FCoord* coord) const override;
};