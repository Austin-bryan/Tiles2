#pragma once
#include "CoreMinimal.h"
#include "Populators/BoardPopulator.h"
#include "ForwardDeclares.h"

struct FCoord;
enum class EBoardShape;

class TILES2_API HexBoardPopulator : public BoardPopulator
{
public:
	HexBoardPopulator(ABoard* const board, const EBoardShape& shape) :
		BoardPopulator(board, shape){ }
	void Populate(FCoordPtr, Tiles& tiles) override;
};