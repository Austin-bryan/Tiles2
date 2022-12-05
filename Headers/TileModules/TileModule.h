#pragma once
#include "CoreMinimal.h"
#include "Enums.h"
#include "TileModule.generated.h"

class AModTile;
enum class EModule : uint8;
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
	void Init() const;
	virtual void ApplyParameters(const TArray<FParameter>& parameters){ }
	
	template<class T>
	static UTileModule* Create(AModTile* tile, const TArray<FParameter>& parameters);
	
	virtual EModule ModuleType() const { return EModule::Normal; }
protected:
	AModTile* ModTile;
};