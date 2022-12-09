#pragma once
#include "CoreMinimal.h"
#include "Enums.h"
#include "Tile.h"
#include "TileModule.generated.h"

class UPaperSprite;
class AModTile;
class UPaperSpriteComponent;
enum class EModule    : uint8;
enum class ETileColor : uint8;
struct FParameter;

/**
 * Modifies Player and CreatorTiles to look different and have different gameplay elements stacked on top of one another.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TILES2_API ATileModule : public AActor, public ITileComponent
{
	GENERATED_BODY()
public:
	ATileModule();
	void BeginPlay() override;
	virtual void Init() const;
	virtual void ApplyParameters(const TArray<FParameter>& parameters){ }
	
	template<class T>
	static ATileModule* Create(AModTile* tile, const TArray<FParameter>& parameters);
	
	FString GetSpritePath()      const;
	virtual int SpriteOrder()    const { return 0; }
	virtual EModule ModuleType() const { return EModule::Normal; }
	void Tick(float DeltaSeconds) override;
protected:
	UPROPERTY()
		AModTile* ModTile;
	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
		UPaperSpriteComponent* Sprite;
	virtual UPaperSprite* GetSprite() const;
};