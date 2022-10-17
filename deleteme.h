#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "deleteme.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TILES2_API Udeleteme : public USceneComponent
{
	GENERATED_BODY()

public:	
	Udeleteme();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
