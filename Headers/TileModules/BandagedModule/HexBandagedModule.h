#pragma once
#include "CoreMinimal.h"
#include "BandagedModule.h"
#include "HexBandagedModule.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TILES2_API UHexBandagedModule : public UBandagedModule	
{
	GENERATED_BODY()
public:
	UHexBandagedModule() : UBandagedModule(){}
	~UHexBandagedModule(){}
	void ApplyParameters(const TArray<FParameter>& parameters) override;
};