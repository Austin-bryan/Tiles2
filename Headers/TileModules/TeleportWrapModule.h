#pragma once
#include "CoreMinimal.h"
#include "TileModule.h"
#include "TeleportWrapModule.generated.h"

/**
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TILES2_API ATeleportWrapModule : public ATileModule	
{
	GENERATED_BODY()
public:
	ATeleportWrapModule() : ATileModule(){}
	
	void Init() const override;
	EModule ModuleType() const override { return EModule::TeleportWrap; }
};

