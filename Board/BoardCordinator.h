#pragma once

#include "CoreMinimal.h"
#include "Board/Board.h"
#include "Components/ActorComponent.h"
#include "BoardCordinator.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TILES2_API UBoardCordinator : public UActorComponent
{
	GENERATED_BODY()
public:	
	UBoardCordinator(){}
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;
private:
};
