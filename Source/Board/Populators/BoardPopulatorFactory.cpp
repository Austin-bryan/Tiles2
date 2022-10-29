#include "Populators/BoardPopulatorFactory.h"
#include "Board.h"
#include "BoardPopulator.h"
#include "Populators/HexBoardPopulator.h"
#include "Populators/TriBoardPopulator.h"
#include "Populators/SqrBoardPopulator.h"

BoardPopulatorFactory::BoardPopulatorFactory() { }
BoardPopulator* BoardPopulatorFactory::Create(ABoard* const board)
{
	switch (const auto shape = board->GetBoardShape())
	{
	case EBoardShape::Hex:		return new HexBoardPopulator(board);      
	case EBoardShape::Triangle: return new TriBoardPopulator(board); 
	default:					return new SqrBoardPopulator(board);   
	}
}