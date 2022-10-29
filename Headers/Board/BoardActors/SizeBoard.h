#pragma once
#include "CoreMinimal.h"
#include "Board.h"
#include "SizeBoard.generated.h"

class ATile;
enum class EBoardShape;

UCLASS()
class TILES2_API ASizeBoard : public ABoard
{
	GENERATED_BODY()
	UClass* TileClass() const override;
};