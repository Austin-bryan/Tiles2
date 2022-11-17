#pragma once
#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "MeshGenerator.generated.h"

UCLASS()
class TILES2_API AMeshGenerator : public AActor
{
    GENERATED_BODY()
public:
    AMeshGenerator();
    void BeginPlay() override;

    UPROPERTY(EditAnywhere)
        float Size;
    UPROPERTY(VisibleAnywhere)
        USceneComponent* Root;
    UPROPERTY(VisibleAnywhere)
        UProceduralMeshComponent* Mesh;
};