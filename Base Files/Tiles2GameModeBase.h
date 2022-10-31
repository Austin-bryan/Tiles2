#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Tiles2GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TILES2_API ATiles2GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
    void Tick(float DeltaSeconds) override;
};
