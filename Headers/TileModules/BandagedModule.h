#pragma once
#include "CoreMinimal.h"
#include "TileModule.h"
#include "BandagedModule.generated.h"

/**
 * BandagedTiles are normal tiles stuck together to move as one.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TILES2_API ABandagedModule : public ATileModule	
{
	GENERATED_BODY()
public:
	ABandagedModule();
	~ABandagedModule() { }
	EModule ModuleType() const override { return EModule::Bandaged; }

	void Tick(float DeltaSeconds) override;
	TSharedPtr<TArray<AModTile*>> Tiles;

	UPROPERTY(VisibleAnywhere)
	FString TileIDs;
};