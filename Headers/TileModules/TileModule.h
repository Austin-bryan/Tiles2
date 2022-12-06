#pragma once
#include "CoreMinimal.h"
#include "Enums.h"
#include "TileModule.generated.h"

class UPaperSprite;
class AModTile;
class UPaperSpriteComponent;
enum class EModule : uint8;
struct FParameter;

/**
 * Modifies Player and CreatorTiles to look different and have different gameplay elements stacked on top of one another.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TILES2_API ATileModule : public AActor
{
	GENERATED_BODY()
public:
	ATileModule();
	void BeginPlay() override;
	virtual void Init() const;
	virtual void ApplyParameters(const TArray<FParameter>& parameters){ }
	
	template<class T>
	static ATileModule* Create(AModTile* tile, const TArray<FParameter>& parameters);
	
	virtual EModule ModuleType() const { return EModule::Normal; }
	virtual int SpriteOrder() const { return 0; }
	FString GetSpritePath() const;

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