#pragma once
#include "CoreMinimal.h"

class ABoard;
class BoardPopulator;

/**
 * 
 */
class TILES2_API BoardPopulatorFactory
{
public:
	BoardPopulatorFactory();
	static BoardPopulator* Create(ABoard* const board)
};
