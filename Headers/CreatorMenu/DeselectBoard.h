#pragma once
#include "CoreMinimal.h"
#include "DeselectBoard.generated.h"

class UBoxComponent;

/*
 * When the player touches this, it deslects the entire CreatorBoard.
 */
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