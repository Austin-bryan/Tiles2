#pragma once
#include "CoreMinimal.h"
#include "BandagedModule.h"
#include "SqrBandagedModule.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TILES2_API USqrBandagedModule : public UBandagedModule	
{
	GENERATED_BODY()
public:
	USqrBandagedModule() : UBandagedModule(){}
	~USqrBandagedModule(){}
	void ApplyParameters(const TArray<FParameter>& parameters) override;
};