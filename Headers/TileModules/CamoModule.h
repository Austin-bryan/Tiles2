#pragma once
#include "CoreMinimal.h"
#include "TileModule.h"
#include "CamoModule.generated.h"

/**
 * Always are correct, changing their colors to match their targettile.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TILES2_API ACamoModule : public ATileModule	
{
	GENERATED_BODY()
public:
	ACamoModule() : ATileModule(){}
	~ACamoModule();
	void ApplyCamoTexture(const AModTile* tile) const;

	void Init() const override;
	void BeginPlay() override;
	EModule ModuleType() const override { return EModule::Camo; }
};