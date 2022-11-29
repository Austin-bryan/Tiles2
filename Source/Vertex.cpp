#include "Vertex.h"
#include "Logger.h"

TArray<Vertex*> Vertex::Vertices;
int Vertex::Count;

bool Vertex::AreNeighbors(const Vertex* a, const Vertex* b)
{
    FVector va = a->GetWorldPosition();
    FVector vb = b->GetWorldPosition();
    va.Y = 0;
    vb.Y = 0;

    return FVector::Distance(a->GetWorldPosition(), b->GetWorldPosition()) < 30;
}
bool Vertex::operator==(const Vertex* rhs) const { return *this == *rhs; }
bool Vertex::operator==(const Vertex rhs)  const { return ID == rhs.ID; }

// this needs to be a delay inorder for for the positions to be accurate
void Vertex::LinkVertices()
{
    for (const auto other : Vertices)
    {
        if (other == this || !AreNeighbors(this, other))
            continue;
        neighbors.AddUnique(other);
        other->neighbors.AddUnique(this);
    }
}
Vertex::Vertex(const int _vertexIndex, const int _sideCount, const FVector _position, UMeshGenerator* _generator) :
    vertexIndex{ _vertexIndex }, sideCount{ _sideCount }, generator{ _generator }, position{ _position }
{
    ID = Count;
    Count++;
    Vertices.AddUnique(this);
}

bool Vertex::IsMerged()            const { return hasBeenMerged; }
ATile*  Vertex::GetTile()          const { return Cast<ATile>(generator->GetOwner());  }
Vertex* Vertex::PrevVertex()       const { return generator->vertices[(FMath::Abs(vertexIndex - 1 + sideCount) % sideCount)]; }
Vertex* Vertex::NextVertex()       const { return generator->vertices[(vertexIndex + 1) % sideCount]; }
FVector Vertex::GetLocalPosition() const { return position; }
FVector Vertex::GetWorldPosition() const { return generator->GetOwner()->GetActorTransform().TransformPosition(position); }

void Vertex::ApplyPosition() { SetPosition(queuedPosition); }
void Vertex::QueuePosition(const FVector newPosition) { hasBeenMerged = true, queuedPosition = newPosition; }
void Vertex::SetPosition(const FVector newPosition)
{
    position = generator->GetOwner()->GetTransform().InverseTransformPosition(newPosition), hasBeenMerged = true;
    position.Y = 0;
    // generator->vertexPositions[vertexIndex] = position;

    // const auto normalizedPos = position;
    // const float length = 0.2f;

    generator->vertexPositions[vertexIndex] = position;
    UMeshGenerator::Generators.AddUnique(generator);
}
