#include "Vertex.h"

Vertex::Vertex(const int _vertexIndex, const FVector vertexPos, UMeshGenerator* _generator)
{
    vertexIndex = _vertexIndex,generator = _generator, position = vertexPos;
}

bool Vertex::IsMerged() const { return hasBeenMerged; }
FVector Vertex::GetPosition() const { return position; }

void Vertex::SetPosition(const FVector newPosition)
{
    position = newPosition, hasBeenMerged = true;
    generator->vertices[vertexIndex] = position;
    generator->UpdateMesh(0);
}
