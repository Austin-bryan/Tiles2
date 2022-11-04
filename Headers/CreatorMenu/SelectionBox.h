#pragma once
#include "CoreMinimal.h"
#include "SelectionBox.generated.h"

class ACreatorTile;
class UBoxComponent;

UCLASS()
class ASelectionBox : public AActor
{
    GENERATED_BODY()
public:
    ASelectionBox();
    void BeginPlay() override;
    void SetVisibility(bool visibility);
    void ScaleArea(float width, float height);

    void Tick(float DeltaSeconds) override;
protected:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* mesh;
    
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* collider;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* root;
private:
    bool isVisible;

    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);
    
    UFUNCTION()
    void OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor,  UPrimitiveComponent* otherComp, int otherBodyIndex);

    void Select(AActor* otherActor, bool isSelected);
};