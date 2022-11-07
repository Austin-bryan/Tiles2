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
    void SetVisibility(bool _isVisible, bool _isRotating = false);
    void ScaleArea(float width, float height) const;
protected:
    UPROPERTY(VisibleAnywhere)
        UStaticMeshComponent* mesh;
    UPROPERTY(VisibleAnywhere)
        UStaticMeshComponent* collider;
    UPROPERTY(VisibleAnywhere)
        USceneComponent* root;
private:
    bool isVisible, isRotating;

    UFUNCTION()
        void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);
    UFUNCTION()
        void OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor,  UPrimitiveComponent* otherComp, int otherBodyIndex);
    void Select(AActor* otherActor, bool isSelected) const;
};