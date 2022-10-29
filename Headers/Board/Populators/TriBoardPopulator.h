#pragma once
#include "CoreMinimal.h"
#include "Populators/BoardPopulator.h"
#include "ForwardDeclares.h"

class ATile;
class ABoard;

class TILES2_API TriBoardPopulator : public BoardPopulator
{
public:
	explicit TriBoardPopulator(ABoard* const board) : BoardPopulator(board){ }
	void Populate(FCoordPtr, Tiles& tiles) override;
};