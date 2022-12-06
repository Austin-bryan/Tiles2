#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoardCoordinator.generated.h"

class ABoard;
class APlayerBoard;
class ACreatorBoard;
class ASizeBoard;

UENUM()
enum class EBoardType : uint8 { PlayerBoard, CreatorBoard, SizeBoard, TargetBoard  };

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TILES2_API UBoardCoordinator : public UActorComponent
{
	GENERATED_BODY()
public:	
	UBoardCoordinator(){}
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetBoard(ABoard* board, EBoardType boardType);

	APlayerBoard* PlayerBoard() const { return playerBoard; }
protected:
	UPROPERTY()
		APlayerBoard* playerBoard;
	UPROPERTY()
		ACreatorBoard* creatorBoard;
	UPROPERTY()
		ASizeBoard* sizeBoard;
};
