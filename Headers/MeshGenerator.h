#pragma once
#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "MeshGenerator.generated.h"

class Vertex;

UCLASS()
class TILES2_API UMeshGenerator : public USceneComponent
{
    friend class Vertex;
    GENERATED_BODY()
public:
    UMeshGenerator(); 
    void BeginPlay() override;

        float Size = 5.5f;
        FVector Lengths = FVector(10, 10, 10);
#ifdef SHOW_VERTICES 
    void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
#endif
    UPROPERTY(VisibleAnywhere)
        USceneComponent* Root;
    UPROPERTY(VisibleAnywhere)
        UProceduralMeshComponent* Mesh;
    UPROPERTY(EditAnywhere, Category="Mesh Generation")
        float MeshDistance = 0;
    static float distance;

    UFUNCTION(BlueprintCallable)
        static void Merge();
    static TArray<Vertex> UniversalVertices;
private:
    void DrawHex(int index, FRotator faceAngle, FVector origin);
    void DrawQuad(const int index, const int width, const int height, const FRotator faceAngle, const FVector origin);
    void UpdateMesh(int index);
    void ClearData();

    TArray<FVector> vertices;
    TArray<int> triangles;
    TArray<FVector> normals;
    TArray<FVector2D> UV;
    TArray<FColor> colors;
    TArray<FProcMeshTangent> tangents;
};