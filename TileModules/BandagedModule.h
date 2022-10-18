#pragma once
#include "CoreMinimal.h"
#include "TileModule.h"
#include "BandagedModule.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TILES2_API UBandagedModule : public UTileModule	
{
	GENERATED_BODY()
public:
	UBandagedModule() : UTileModule(){}
	~UBandagedModule();
	void ApplyParameters(const TArray<FParameter>& parameters) override;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};