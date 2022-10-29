#pragma once
#include "CoreMinimal.h"
#include "Board.h"
#include "PlayerBoard.generated.h"

UCLASS()
class TILES2_API APlayerBoard : public ABoard
{
	GENERATED_BODY()
	UClass* TileClass() const override;
};