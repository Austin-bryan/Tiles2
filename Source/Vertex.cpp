#include "Vertex.h"

#include "Logger.h"

Vertex::Vertex(const int _vertexIndex, const FVector vertexPos, UMeshGenerator* _generator)
{
    vertexIndex = _vertexIndex, generator = _generator, position = vertexPos;
}
bool Vertex::IsMerged() const { return hasBeenMerged; }

FVector Vertex::GetWorldPosition() const { return position + generator->GetOwner()->GetActorLocation(); }
FVector Vertex::GetLocalPosition() const { return position; }

void Vertex::SetPosition(const FVector newPosition)
{
    position = newPosition - generator->GetOwner()->GetActorLocation(), hasBeenMerged = true;
    position.Y = 0;
    generator->vertices[vertexIndex] = position;
    generator->UpdateMesh(0);
}