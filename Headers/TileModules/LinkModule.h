#pragma once
#include "CoreMinimal.h"
#include "TileModule.h"
#include "LinkModule.generated.h"

/**
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TILES2_API ALinkModule : public ATileModule	
{
	GENERATED_BODY()
public:
	ALinkModule() : ATileModule(){}
	
	void Init() const override;
	int SpriteOrder() const override { return 5; }
	EModule ModuleType() const override { return EModule::Link; }
};

