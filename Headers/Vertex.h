#pragma once
#include "CoreMinimal.h"
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
    FVector GetWorldPosition() const;
    FVector GetLocalPosition() const;
    Vertex NextVertex() const;
    Vertex PrevVertex() const;

    void ApplyPosition();
    void QueuePosition(const FVector newPosition);
    void SetPosition(const FVector newPosition);
private:
    int vertexIndex, sideCount;
    UMeshGenerator* generator;
    FVector position, queuedPosition;
    bool hasBeenMerged = false;
};

inline bool operator==(
    const Vertex& rhs,
    const Vertex& lhs)
{
    return rhs.GetWorldPosition() == lhs.GetWorldPosition();
}