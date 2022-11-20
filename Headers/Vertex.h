#pragma once
#include "CoreMinimal.h"
#include "Logger.h"
#include "MeshGenerator.h"

class AMeshGenerator;

class TILES2_API Vertex
{
public:
    Vertex(const int _vertexIndex, const FVector vertexPos, AMeshGenerator* _generator)
    {
        vertexIndex = _vertexIndex,generator = _generator, position = vertexPos;
    }
    bool IsMerged()            const { return hasBeenMerged; }
    FVector GetLocalPosition() const { return position; }
    FVector GetWorldPosition() const { return position + generator->GetActorLocation(); }
    FVector GetGeneratorPos()  const { return generator->GetActorLocation(); }

    void SetWorldPosition(const FVector newPosition)
    {
        position = newPosition - generator->GetActorLocation();
        hasBeenMerged = true;
        generator->vertices[vertexIndex] = position;
        generator->UpdateMesh(0);
    }
    void SetLocalPosition(const FVector newPosition)
    {
        position = newPosition;
        hasBeenMerged = true;
        generator->vertices[vertexIndex] = position;
        generator->UpdateMesh(0);
    }
private:
    int vertexIndex;
    AMeshGenerator* generator;
    FVector position;
    bool hasBeenMerged = false;
};