#pragma once
#include "CoreMinimal.h"
#include "MeshGenerator.h"

class UMeshGenerator;

class TILES2_API Vertex
{
public:
    Vertex(const int _vertexIndex, const FVector vertexPos, UMeshGenerator* _generator);
    
    bool IsMerged() const;
    FVector GetWorldPosition() const;
    FVector GetLocalPosition() const;
    void SetPosition(const FVector newPosition);
private:
    int vertexIndex;
    UMeshGenerator* generator;
    FVector position;
    bool hasBeenMerged = false;
};