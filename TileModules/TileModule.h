#pragma once
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Tile.h"
#include "TileModule.generated.h"

struct FParameter;

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
	virtual void ApplyParameters(const TArray<FParameter>& parameters){}
	
	template<class T>
	static UTileModule* Create(ATile* tile, const TArray<FParameter>& parameters);
protected:
	ATile* ModTile;
};