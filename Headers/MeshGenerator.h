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

    UPROPERTY(EditAnywhere) float Size;
    UPROPERTY(EditAnywhere) int xLength = 10;
    UPROPERTY(EditAnywhere) int yLength = 10;
    UPROPERTY(EditAnywhere) int zLength = 10;
        
    UPROPERTY(VisibleAnywhere)
        USceneComponent* Root;
    UPROPERTY(VisibleAnywhere)
        UProceduralMeshComponent* Mesh;
private:
    void DrawQuad(const FRotator rotator, const FVector origin);
    void ClearData();

    TArray<FVector> vertices;
    TArray<int> triangles;
    TArray<FVector> normals;
    TArray<FVector2D> UV;
    TArray<FColor> colors;
    TArray<FProcMeshTangent> tangents;
};