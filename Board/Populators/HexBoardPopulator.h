#pragma once
#include "CoreMinimal.h"
#include "Board/Populators/BoardPopulator.h"
#include "ForwardDeclares.h"

struct FCoord;
enum class EBoardShape;

class TILES2_API HexBoardPopulator : public BoardPopulator
{
public:
	HexBoardPopulator(ABoard* const board, const EBoardShape& shape) :
		BoardPopulator(board, shape){ }
	void Populate(FCoordPtr, Tiles& tiles) override;

	float GetSpaceX() const override { return 45; }
	float GetSpaceZ() const override { return 50; }
	float GetOffsetX(FCoordPtr coord) const override;
	float GetOffsetZ(FCoordPtr coord) const override;
};