#pragma once
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Tile.h"
#include "TileModule.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TILES2_API UTileModule : public USceneComponent
{
	GENERATED_BODY()
public:
	UTileModule();
	~UTileModule();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	template<class T>
	static UTileModule* Create(ATile* tile);
protected:
	ATile* ModTile;
};