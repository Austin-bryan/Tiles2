#include "Board/Populators/BoardPopulatorFactory.h"
#include "Board.h"
#include "BoardPopulator.h"
#include "Board/Populators/HexBoardPopulator.h"
#include "Board/Populators/TriangleBoardPopulator.h"
#include "Board/Populators/SquareBoardPopulator.h"

BoardPopulatorFactory::BoardPopulatorFactory() { }
BoardPopulator* BoardPopulatorFactory::Create(ABoard* const board)
{
	switch (const auto shape = board->GetBoardShape())
	{
	case EBoardShape::Hex:		return new HexBoardPopulator(board, shape);      
	case EBoardShape::Triangle: return new TriangleBoardPopulator(board, shape); 
	default:					return new SquareBoardPopulator(board, shape);   
	}
}