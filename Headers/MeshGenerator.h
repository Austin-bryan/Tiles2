#pragma once
#include "CoreMinimal.h"
#include "CreatorBoard.h"
#include "ProceduralMeshComponent.h"
#include "Vertex.h"
#include "MeshGenerator.generated.h"

#define DRAW_DEBUG

class ATile;
class Vertex;
class ACreatorTile;

UCLASS()
class TILES2_API UMeshGenerator : public UActorComponent
{
    friend class Vertex;
    GENERATED_BODY()
public:
    UMeshGenerator(); 
#ifdef DRAW_DEBUG 
    void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction)
    override;
#endif
    UPROPERTY(VisibleAnywhere)
        UProceduralMeshComponent* ProceduralMesh;
    const static float distance;

    UFUNCTION(BlueprintCallable)
        static void Merge();
    static TArray<Vertex> UniversalVertices;
    static TArray<UMeshGenerator*> Generators;
    static TArray<ACreatorTile*> TilesToMerge;
    static void AverageVertices(
        TArray<Vertex*> neighbors,
        FVector sum);
    static FVector GetEndVertex(
        Vertex start,
        Vertex end);
    static bool GetIntersection(
        UWorld* worldContext,
        Vertex startA,
        Vertex endA,
        Vertex startB,
        Vertex endB,
        FVector& intersection);
    static bool IsIntersectionValid(
        EVertexMode vertexMode,
        const ACreatorTile* const creatorTileA,
        const ACreatorTile* const creatorTileB,
        const Vertex& vertexA,
        const Vertex& vertexB,
        FVector& intersection);
    static void QueueVertices(
        TArray<Vertex*>& queuedVertices,
        ACreatorTile* const& creatorTileA,
        Vertex& vertexA,
        Vertex& vertexB,
        FVector intersection);
    static bool ShouldMergeVertices(
        const Vertex& vertexA,
        const Vertex& vertexB);
    static void MergeWithNeighbor(
        TArray<Vertex*>& queuedVertices,
        ACreatorTile* const& creatorTileA,
        Vertex& vertexA,
        FVector& sum,
        ACreatorTile* const& creatorTileB);
    static void MergeWithNeighbors(
        TArray<Vertex*>& queuedVertices,
        ACreatorTile* const& creatorTileA);

    void Init(
        int _radius,
        int _vertexCount,
        int _angleOffset,
        int _angle);
private:
    void Draw();
    void UpdateMesh();
    void ClearData();

    TArray<Vertex> vertices;
    TArray<FVector> vertexPositions;
    TArray<int> triangles;
    TArray<FVector> normals;
    TArray<FVector2D> UV;
    TArray<FColor> colors;
    TArray<FProcMeshTangent> tangents;
    
    int radius, angle, vertexCount, angleOffset;
};