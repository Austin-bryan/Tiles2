#pragma once
#include "CoreMinimal.h"
#include "TileModule.h"
#include "CamoModule.generated.h"

/**
 * Always are correct, changing their colors to match their targettile.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TILES2_API UCamoModule : public UTileModule	
{
	GENERATED_BODY()
public:
	UCamoModule() : UTileModule(){}
	~UCamoModule();

	void BeginPlay() override;
	EModule ModuleType() const override { return EModule::Camo; }
};