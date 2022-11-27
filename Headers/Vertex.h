#pragma once
#include "CoreMinimal.h"
#include "Logger.h"
#include "MeshGenerator.h"

class UMeshGenerator;

class TILES2_API Vertex
{
public:
    Vertex(
        const int _vertexIndex,
        const int _sideCount,
        const FVector _position,
        UMeshGenerator* _generator);
    
    bool IsMerged() const;
    Vertex* NextVertex() const;
    Vertex* PrevVertex() const;
    FVector GetLocalPosition() const;
    FVector GetWorldPosition() const;

    void LinkVertices();
    void ApplyPosition();
    void QueuePosition(const FVector newPosition);
    void SetPosition(const FVector newPosition);

    static bool AreNeighbors(
        const Vertex* a,
        const Vertex* b);
    bool operator==(
        const Vertex* rhs) const;
    bool operator==(
        const Vertex rhs) const;
    
    static TArray<Vertex*> Vertices;
    TArray<Vertex*> linkedVertices;
    int vertexIndex, sideCount;
    UMeshGenerator* generator;
    FVector position, queuedPosition;
    bool hasBeenMerged = false;
    static int Count;

private:
    int ID;

};

