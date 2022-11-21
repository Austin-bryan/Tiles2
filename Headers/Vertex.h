#pragma once
#include "CoreMinimal.h"
#include "AssetDir.h"
#include "Logger.h"
#include "MeshGenerator.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"

class UMeshGenerator;

class TILES2_API Vertex
{
public:
    Vertex(const int _vertexIndex, const FVector vertexPos, UMeshGenerator* _generator)
    {
        vertexIndex = _vertexIndex,generator = _generator, position = vertexPos;
    }
    bool IsMerged()            const { return hasBeenMerged; }
    FVector GetLocalPosition() const { return position; }
    FVector GetWorldPosition() const { return position; }
    // FVector GetWorldPosition() const { return position + generator->GetOwner()->GetActorLocation(); }
    FVector GetGeneratorPos()  const { return generator->GetOwner()->GetActorLocation(); }

    void SetWorldPosition(const FVector newPosition)
    {
        position = newPosition;
        // position = newPosition - GetGeneratorPos();
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
    UMeshGenerator* generator;
    FVector position;
    bool hasBeenMerged = false;
};