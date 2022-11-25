#pragma once
#include "CoreMinimal.h"
#include "CreatorBoard.h"
#include "ProceduralMeshComponent.h"
#include "MeshGenerator.generated.h"

// #define DRAW_DEBUG

class ATile;
class AVertex;
class ACreatorTile;

UCLASS()
class TILES2_API UMeshGenerator : public UActorComponent
{
    friend class AVertex;
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
    static TArray<AVertex> UniversalVertices;
    static TArray<UMeshGenerator*> Generators;
    static TArray<ACreatorTile*> TilesToMerge;
    static void AverageVertices(
        TArray<AVertex*> neighbors,
        FVector sum);
    static FVector GetEndVertex(
        const AVertex* start,
        const AVertex* end);
    static bool GetIntersection(
        UWorld* worldContext,
        const AVertex* startA,
        const AVertex* endA,
        const AVertex* startB,
        const AVertex* endB,
        FVector& intersection);
    static bool IsIntersectionValid(
        EVertexMode vertexMode,
        const ACreatorTile* const creatorTileA,
        const ACreatorTile* const creatorTileB,
        const AVertex* vertexA,
        const AVertex* vertexB,
        FVector& intersection);
    static void QueueVertices(
        TArray<AVertex*>& queuedVertices,
        ACreatorTile* const& creatorTileA,
        AVertex* vertexA,
        AVertex* vertexB,
        FVector intersection);
    static bool ShouldMergeVertices(
        const AVertex* vertexA,
        const AVertex* vertexB);
    static void MergeWithNeighbor(
        TArray<AVertex*>& queuedVertices,
        ACreatorTile* const& creatorTileA,
        AVertex* vertexA,
        FVector& sum,
        ACreatorTile* const& creatorTileB);
    static void MergeWithNeighbors(
        TArray<AVertex*>& queuedVertices,
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

    TArray<AVertex*> vertices;
    TArray<FVector> vertexPositions;
    TArray<int> triangles;
    TArray<FVector> normals;
    TArray<FVector2D> UV;
    TArray<FColor> colors;
    TArray<FProcMeshTangent> tangents;
    
    int radius, angle, vertexCount, angleOffset;
};