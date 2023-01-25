#include "Vertex.h"
#include "Logger.h"

TArray<Vertex*> Vertex::Vertices;

float Vertex::Distance   (const Vertex* a, const Vertex* b) { return FVector::Distance(a->GetWorldPosition(), b->GetWorldPosition()); }
bool Vertex::AreNeighbors(const Vertex* a, const Vertex* b) { return Distance(a, b) < 30; }
bool Vertex::AreCollinear(const Vertex* a, const Vertex* b, const Vertex* c)
{
    return (a->Y() - b->Y()) * (a->X() - c->X()) == (a->Y() - c->Y()) * (a->X() - b->X());     
}

bool Vertex::operator==(const Vertex* rhs) const { return *this == *rhs; }
bool Vertex::operator==(const Vertex rhs)  const { return ID == rhs.ID; }

void Vertex::SetupNeighbors()
{
    for (const auto other : Vertices)
    if (other != this && AreNeighbors(this, other))
    {
        neighbors.AddUnique(other);
        other->neighbors.AddUnique(this);
    }
}
Vertex::Vertex(int index, int sideCount, FVector position, UMeshGenerator* generator, int maxIndex, int indexIncrement) :
    index{ index }, sideCount{ sideCount }, maxIndex{ maxIndex }, indexIncrement{ indexIncrement }, generator{ generator }, position{ position }
{
    ID = Vertices.Num();
    Vertices.AddUnique(this);
    unmergedPosition = position;
}

bool    Vertex::IsMerged()          const { return hasBeenMerged; }
bool    Vertex::ShouldRound()       const { return !IsMerged() || Shape() == EBoardShape::Triangle && mergeCount <= 2; }
ATile*  Vertex::GetTile()           const { return Cast<ATile>(generator->GetOwner());  }
Vertex* Vertex::PrevVertex()        const { return generator->vertices[(FMath::Abs(index - indexIncrement + maxIndex) % (maxIndex))]; }
Vertex* Vertex::NextVertex()        const { return generator->vertices[(index + indexIncrement) % (maxIndex)]; }
FVector Vertex::GetLocalPosition()  const { return position; }
FVector Vertex::GetWorldPosition()  const { return generator->GetOwner()->GetActorTransform().TransformPosition(position); }
FVector Vertex::GetQueuedPosition() const { return queuedPosition; }

bool Vertex::IsOnBoardEdge() const
{
    int neighborCount = neighbors.Num();
	
    return Shape() == EBoardShape::Triangle && neighborCount != 5
        || Shape() == EBoardShape::Square 	&& neighborCount != 3
        || Shape() == EBoardShape::Hex    	&& neighborCount != 2;
}
void Vertex::Unmerge()                        { hasBeenMerged = false, position = unmergedPosition, mergeCount = 0; }
void Vertex::ApplyPosition()                  { SetPosition(queuedPosition); }
void Vertex::SetShouldRound(const bool value) { shouldRoundWhileMerged = value; }
void Vertex::QueuePosition(const FVector newPosition, int _mergeCount) { hasBeenMerged = true, queuedPosition = newPosition, mergeCount = _mergeCount; }
void Vertex::SetPosition  (const FVector newPosition, int _mergeCount)
{
    if (_mergeCount > 0)
        mergeCount = _mergeCount;
    position = generator->GetOwner()->GetTransform().InverseTransformPosition(newPosition), hasBeenMerged = true;
    position.Y = 0;
}
