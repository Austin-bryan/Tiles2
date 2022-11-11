#pragma once
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Tile.h"
#include "TileModule.generated.h"

struct FParameter;

/**
 * Modifies Player and CreatorTiles to look different and have different gameplay elements stacked on top of one another.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TILES2_API UTileModule : public USceneComponent
{
	GENERATED_BODY()
public:
	UTileModule();
	~UTileModule();

	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void ApplyParameters(const TArray<FParameter>& parameters){}
	
	template<class T>
	static UTileModule* Create(ATile* tile, const TArray<FParameter>& parameters);
protected:
	UPROPERTY()
		ATile* ModTile;
};