#pragma once
#include "CoreMinimal.h"
#include "MeshGenerator.h"

class UMeshGenerator;

class TILES2_API Vertex
{
    friend class AStaticResetter;
public:
    Vertex(
        const int _vertexIndex, const int _sideCount,
        const FVector _position, UMeshGenerator* _generator);
    
    ATile*  GetTile() const;
    bool    IsMerged() const;
    bool    ShouldRound() const;
    Vertex* NextVertex() const;
    Vertex* PrevVertex() const;
    FVector GetLocalPosition() const;
    FVector GetWorldPosition() const;
    FVector GetQueuedPosition() const;
    void Unmerge();
    const TArray<Vertex*>& Neighbors() const { return neighbors; }

    float X() const { return GetWorldPosition().X; }
    float Y() const { return GetWorldPosition().Y; }
    float Z() const { return GetWorldPosition().Z; }

    void LinkVertices();
    void ApplyPosition();
    void QueuedPosition(const FVector newPosition);
    void SetPosition(const FVector newPosition);
    void SetShouldRound(bool value);

    static bool AreNeighbors(
        const Vertex* a,
        const Vertex* b);
    static bool AreCollinear(
        const Vertex* a,
        const Vertex* b,
        const Vertex* c);
    bool operator==(
        const Vertex* rhs) const;
    bool operator==(
        const Vertex rhs) const;
private:
    static TArray<Vertex*> Vertices;

    int ID, vertexIndex, sideCount;
    bool hasBeenMerged = false, shouldRoundWhileMerged = false;
    
    TArray<Vertex*> neighbors;
    UMeshGenerator* generator;
    FVector position, queuedPosition;
    FVector unmergedPosition;
};

