#pragma once
#include "Vertex.h"
#include "CoreMinimal.h"
#include "CreatorBoard.h"
#include "MeshGenerator.generated.h"

// #define DRAW_DEBUG

class AModTile;
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
    UFUNCTION(BlueprintCallable)
        static void Merge();
    UFUNCTION(BlueprintCallable)
        static void Unmerge();
    
    static ETileColor GetBandagedColor();
    static TArray<UMeshGenerator*> Generators;
    
    static void MergeTwoVertices(
        TArray<Vertex*>& queuedVertices,
        ACreatorTile* const& creatorTileA,
        Vertex* vertexA);
    static void MergeThreeVertices(
        TArray<Vertex*>& queuedVertices,
        ACreatorTile* const& creatorTileA,
        Vertex* vertexA,
        TArray<ACreatorTile*> selectedNeighbors);
    static void MergeMultipleVertices(
        const Vertex* vertexA);

    static void AverageVertices(
        TArray<Vertex*> neighbors,
        FVector sum);
    static FVector GetEndVertex(
        const Vertex* start,
        const Vertex* end);
    static bool GetIntersection(
        const UWorld* worldContext,
        const Vertex* startA,
        const Vertex* endA,
        const Vertex* startB,
        const Vertex* endB,
        FVector& intersection);
    static bool IsIntersectionValid(
        EVertexMode vertexMode,
        const ACreatorTile* const creatorTileA,
        const ACreatorTile* const creatorTileB,
        const Vertex* vertexA,
        const Vertex* vertexB,
        FVector& intersection);
    static void QueueVertices(
        TArray<Vertex*>& queuedVertices,
        ACreatorTile* const& creatorTileA,
        Vertex* vertexA,
        Vertex* vertexB,
        FVector intersection);
    static bool ShouldMergeVertices(
        const Vertex* vertexA,
        const Vertex* vertexB);
    static bool MergeWithNeighbor(
        TArray<Vertex*>& queuedVertices,
        ACreatorTile* const& creatorTileA,
        Vertex* vertexA,
        ACreatorTile* const& creatorTileB);
    static void MergeWithNeighbors(
        TArray<Vertex*>& queuedVertices,
        ACreatorTile* const& creatorTileA);

    void Init(
        int _radius, int _vertexCount,
        int _angleOffset, int _angle);
    void LinkVertices();
private:
    static TSharedPtr<TArray<AModTile*>> sharedSiblings;
    static TArray<ACreatorTile*> tilesToMerge;

    void Draw(bool resetVertices);
    void UpdateMesh();

    TArray<Vertex*> vertices;
    TArray<FVector> roundedVertices;

    int radius, angle, vertexCount, angleOffset;
};