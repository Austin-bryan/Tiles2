#include "Vertex.h"

#include "Logger.h"

Vertex::Vertex(const int _vertexIndex, const int _sideCount, const FVector _position, UMeshGenerator* _generator) :
    vertexIndex{ _vertexIndex }, sideCount{ _sideCount }, generator{ _generator }, position{ _position } { }
bool Vertex::IsMerged() const { return hasBeenMerged; }

Vertex  Vertex::PrevVertex()       const { return generator->vertices[(FMath::Abs(vertexIndex - 1 + sideCount) % sideCount)]; }
Vertex  Vertex::NextVertex()       const { return generator->vertices[(vertexIndex + 1) % sideCount]; }
FVector Vertex::GetLocalPosition() const { return position; }
FVector Vertex::GetWorldPosition() const { return generator->GetOwner()->GetActorTransform().TransformPosition(position); }

void Vertex::ApplyPosition() { SetPosition(queuedPosition); }
void Vertex::QueuePosition(const FVector newPosition) { hasBeenMerged = true, queuedPosition = newPosition; }
void Vertex::SetPosition(const FVector newPosition)
{
    position = generator->GetOwner()->GetTransform().InverseTransformPosition(newPosition), hasBeenMerged = true;
    position.Y = 0;
    generator->vertexPositions[vertexIndex] = position;
    UMeshGenerator::Generators.AddUnique(generator);
}
