#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoardCoordinator.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TILES2_API UBoardCoordinator : public UActorComponent
{
	GENERATED_BODY()
public:	
	UBoardCoordinator(){}
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;
};