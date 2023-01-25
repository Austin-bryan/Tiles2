#pragma once
#include "CoreMinimal.h"
#include "Populators/BoardPopulator.h"
#include "ForwardDeclares.h"

struct FCoord;
class ABoard;

class TILES2_API SqrBoardPopulator final : public BoardPopulator
{
public:
	explicit SqrBoardPopulator(ABoard* const board) : BoardPopulator(board){ }
	void Populate(FCoordPtr size, TilesMap& tiles) override;
};
