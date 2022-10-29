#pragma once
#include "CoreMinimal.h"
#include "Populators/BoardPopulator.h"
#include "ForwardDeclares.h"

struct FCoord;
enum class EBoardShape;

class TILES2_API HexBoardPopulator : public BoardPopulator
{
public:
	explicit HexBoardPopulator(ABoard* const board) : BoardPopulator(board){ }
	void Populate(FCoordPtr, Tiles& tiles) override;
};