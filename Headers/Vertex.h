#pragma once
#include "CoreMinimal.h"
#include "Logger.h"
#include "MeshGenerator.h"

class UMeshGenerator;

//TODO:: private the fields that don't need to be public

class TILES2_API Vertex
{
public:
    Vertex(
        const int _vertexIndex,
        const int _sideCount,
        const FVector _position,
        UMeshGenerator* _generator);
    
    ATile* GetTile() const;
    bool IsMerged() const;
    bool ShouldRound() const;
    Vertex* NextVertex() const;
    Vertex* PrevVertex() const;
    FVector GetLocalPosition() const;
    FVector GetWorldPosition() const;
    FVector GetQueuedPosition() const;
    float X() const { return GetWorldPosition().X; }
    float Y() const { return GetWorldPosition().Y; }
    float Z() const { return GetWorldPosition().Z; }

    void LinkVertices();
    void ApplyPosition();
    void QueuePosition(const FVector newPosition);
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
    
    static TArray<Vertex*> Vertices;
    TArray<Vertex*> neighbors;
    int vertexIndex, sideCount;
    UMeshGenerator* generator;
    FVector position, queuedPosition;
    bool hasBeenMerged = false;
    static int Count;

private:
    int ID;
    bool shouldRoundWhileMerged = false;
};

