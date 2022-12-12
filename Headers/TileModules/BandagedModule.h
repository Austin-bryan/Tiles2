#pragma once
#include "CoreMinimal.h"
#include "TileModule.h"
#include "BandagedModule.generated.h"

class UTileSideHandler;
class ATile;
/**
 * BandagedTiles are normal tiles stuck together to move as one.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TILES2_API ABandagedModule : public ATileModule	
{
	GENERATED_BODY()
	friend class AStaticResetter;
public:
	ABandagedModule();
	~ABandagedModule() { }
	EModule ModuleType() const override { return EModule::Bandaged; }

	void Finish();
	void Init() const override;
	void Tick(float DeltaSeconds) override;
	void AddModTile(AModTile* tile);
	void RemoveModTile(AModTile* tile);
	void SetColor(const ETileColor color, const bool propagate) override;

	// TODO:: make private; add getter
	TArray<AModTile*> Tiles;
	UPROPERTY(VisibleAnywhere)
	FString TileIDs;
protected:
	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* TextRender;
private:
	static int bandageCount;
};