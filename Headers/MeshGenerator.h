#pragma once
#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "TilesMap.h"
#include "Vertex.h"
#include "MeshGenerator.generated.h"

class ACreatorTile;
//#define SHOW_VERTICES
class ATile;
class Vertex;

UCLASS()
class TILES2_API UMeshGenerator : public UActorComponent
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
        UProceduralMeshComponent* ProceduralMesh;
    const static float distance;

    UFUNCTION(BlueprintCallable)
        static void Merge();
    static TArray<Vertex> UniversalVertices;
    static TArray<UMeshGenerator*> Generators;
    static TArray<ACreatorTile*> TilesToMerge;
private:
    void DrawHex(int index, FRotator faceAngle, FVector origin);
    void DrawQuad(const int index, const int width, const int height, const FRotator faceAngle, const FVector origin);
    void UpdateMesh(int index);
    void ClearData();

    TArray<Vertex> vertices;
    TArray<FVector> vertexPositions;
    TArray<int> triangles;
    TArray<FVector> normals;
    TArray<FVector2D> UV;
    TArray<FColor> colors;
    TArray<FProcMeshTangent> tangents;
};