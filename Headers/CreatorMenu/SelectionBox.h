#pragma once
#include "CoreMinimal.h"
#include "SelectionBox.generated.h"

class ACreatorTile;
class UBoxComponent;

/**
 * Used for marquee selection of various shapes dictated by SelectionDrawer.
 * Mesh and Collider are always same shape but are seperated so Mesh can be in the camera plane,
 * while Collider is in the CreatorBoard plane.
 */

UCLASS()
class TILES2_API ASelectionBox : public AActor
{
    GENERATED_BODY()
public:
    ASelectionBox();
    void BeginPlay() override;
    void SetVisibility(bool _isVisible, bool _isRotating = false);
    void ScaleArea(float width, float height) const;
    void SetMesh(FString meshName) const;
protected:
    UPROPERTY(VisibleAnywhere)
        UStaticMeshComponent* mesh;
    UPROPERTY(VisibleAnywhere)
        UStaticMeshComponent* collider;
    UPROPERTY(VisibleAnywhere)
        USceneComponent* root;
    UPROPERTY(EditInstanceOnly)
        bool useBox = true;
private:
    bool isVisible, isRotating;

    UFUNCTION()
        void OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);
    UFUNCTION()
        void OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor,  UPrimitiveComponent* otherComp, int otherBodyIndex);
    void Select(AActor* otherActor, bool isSelected) const;
};