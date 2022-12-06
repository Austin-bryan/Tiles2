#pragma once
#include "CoreMinimal.h"
#include "TileModule.h"
#include "RotatorModule.generated.h"

/**
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TILES2_API ARotatorModule : public ATileModule	
{
	GENERATED_BODY()
public:
	ARotatorModule() : ATileModule(){}
	
	void Init() const override;
	EModule ModuleType() const override { return EModule::Rotator; }

	UPaperSprite* GetSprite() const override;
};

