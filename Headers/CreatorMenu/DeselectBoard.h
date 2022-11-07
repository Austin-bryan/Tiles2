#pragma once
#include "CoreMinimal.h"
#include "DeselectBoard.generated.h"

class UBoxComponent;

UCLASS()
class TILES2_API ADeselectBoard : public AActor
{
    GENERATED_BODY()
public:
    ADeselectBoard();
    void NotifyActorOnClicked(FKey ButtonPressed) override;
protected:
    UPROPERTY(VisibleAnywhere)
    UBoxComponent* Collider;
};
