#pragma once
#include "CoreMinimal.h"
#include "TileModule.h"
#include "CamoModule.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TILES2_API UCamoModule : public UTileModule	
{
	GENERATED_BODY()
public:
	UCamoModule() : UTileModule(){}
	~UCamoModule();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};