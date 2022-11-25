#include "Vertex.h"
#include "MeshGenerator.h"

void AVertex::Init(const int _vertexIndex, const int _sideCount, const FVector _position, UMeshGenerator* _generator)
{
    vertexIndex = _vertexIndex, sideCount = _sideCount, generator = _generator, position = _position;
}
bool AVertex::IsMerged() const { return hasBeenMerged; }

AVertex* AVertex::PrevVertex()       const { return generator->vertices[(FMath::Abs(vertexIndex - 1 + sideCount) % sideCount)]; }
AVertex* AVertex::NextVertex()       const { return generator->vertices[(vertexIndex + 1) % sideCount]; }
FVector  AVertex::GetLocalPosition() const { return position; }
FVector  AVertex::GetWorldPosition() const { return generator->GetOwner()->GetActorTransform().TransformPosition(position); }

void AVertex::ApplyPosition() { SetPosition(queuedPosition); }
void AVertex::QueuePosition(const FVector newPosition) { hasBeenMerged = true, queuedPosition = newPosition; }
void AVertex::SetPosition(const FVector newPosition)
{
    position = generator->GetOwner()->GetTransform().InverseTransformPosition(newPosition), hasBeenMerged = true;
    position.Y = 0;
    generator->vertexPositions[vertexIndex] = position;
    UMeshGenerator::Generators.AddUnique(generator);
}
