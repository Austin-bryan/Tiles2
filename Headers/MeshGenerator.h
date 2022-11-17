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
        float Size = 100;
    UPROPERTY(EditAnywhere)
        FVector Lengths;
    UPROPERTY(VisibleAnywhere)
        USceneComponent* Root;
    UPROPERTY(VisibleAnywhere)
        UProceduralMeshComponent* Mesh;
private:
    void DrawHex(int index, FRotator faceAngle, FVector origin);
    void DrawQuad(const int index, const int width, const int height, const FRotator faceAngle, const FVector origin);
    void ClearData();

    TArray<FVector> vertices;
    TArray<int> triangles;
    TArray<FVector> normals;
    TArray<FVector2D> UV;
    TArray<FColor> colors;
    TArray<FProcMeshTangent> tangents;
};