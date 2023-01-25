#include "AntMarchSpawner.h"
#include "CreatorTile.h"
#include "MeshGenerator.h"
#include "AntMarchLineBase.h"
#include "SeedCreator.h"
#include "TriCoord.h"

class VertexBookkeeper;

/*  Ant Marcher  */
int AntMarchSpawner::Bounds = 1000;

void AntMarchSpawner::Draw(const TMap<FCoord, CopySignature>& _copiedMap)
{
    copiedMap = _copiedMap;
    if (copiedMap.Num() < 1)
        return;

    auto shape = creatorBoard->GetBoardShape();
    int size = shape == EBoardShape::Hex
         ? 6 : shape == EBoardShape::Square
         ? 4 : 6;
    auto bookkeeper = GetOuterVertices(size);   // Vertices that are on the outer perimeter of the copied selection
    TraverseVertices(bookkeeper, size);
    auto antPositions = CenterAntVertices(bookkeeper);

#ifdef SHOW_DEBUG

    bookkeeper.Draw();

    //auto world = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport)->World();
    // for (auto vertexRing : antVertices)
    // for (auto vertex : vertexRing)
        // DrawDebugSphere(world, vertex->GetWorldPosition(), 2, 8, FColor::Yellow, true, -1, 0, 2.f);
    // for (auto vertexRing : antVertices)
    // {
    //     float increment = 255 / (vertexRing.Num() - 1);
    //     for (int i = 0; i < vertexRing.Num(); i++)
    //     {
    //         FVector start = vertexRing[i]->GetWorldPosition();
    //         FVector end = vertexRing[(i + 1) % vertexRing.Num()]->GetWorldPosition();
    //
    //         DrawDebugLine(world, start, end, FColor(increment * i, 0, 255 - increment * i), true, -1, 0, 5);
    //     }
    // }

#endif

    auto world = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport)->World();

    int lineCount = 0;
    int loopCount = 0;
    for (auto vertexRing : antPositions)
    {
        for (int i = 0; i < vertexRing.Num(); i++)
        {
            FVector start = vertexRing[i];
            FVector end = vertexRing[(i + 1) % vertexRing.Num()];

            if (FVector::Distance(start, end) < 0.1f)
                continue;
            AAntMarchLineBase* lineBase;
            if (lineCount < lines.Num())
            {
                lineBase = lines[lineCount];
                lineBase->SetActorLocation(start);
            }
            else
            {
                lineBase = Cast<AAntMarchLineBase>(world->SpawnActor(AAntMarchLineBase::StaticClass(), &start));
                lines.Add(lineBase);
            }
            lineCount++;
            lineBase->SetEndPosition(end);
        }
        loopCount++;
    }
}
void AntMarchSpawner::HideLines()
{
    for (auto line : lines)
        line->Stop();
}
void AntMarchSpawner::BindToSeedChange(ASeedCreator* seedCreator)
{
    seedCreator->OnChangeDetected.BindLambda([&]
    {
        for (auto [coord, signature] : copiedMap)
        {
            auto currentSignature = CopySignature(creatorBoard->At(coord));
            if (currentSignature != signature)
            {
                HideLines();
                return;
            }
        }
    });
}
auto AntMarchSpawner::GetOuterVertices(int size) const -> VertexBookkeeper {

    VertexBookkeeper bookkeeper;
    const auto AddOuterVertex = [&](ACreatorTile* creatorTile, Vertex* vertex)
    {
// #ifdef SHOW_DEBUG
//         creatorTile->SetColor(ETileColor::Blue, true);
// #endif
        bookkeeper.AddOuterVertex(vertex);
    };
    
    for (const auto& [_, copySignature] : copiedMap)
    {
        auto creatorTile = static_cast<ACreatorTile*>(copySignature.GetTile());
        for (auto [__, vertex] : creatorTile->MeshGenerator->Vertices())
        {
            if (vertex->IsOnBoardEdge())
                AddOuterVertex(creatorTile, vertex);
            else for (auto neighbor : vertex->Neighbors())
                if (!static_cast<ACreatorTile*>(neighbor->GetTile())->GetIsSelected() || neighbor->IsOnBoardEdge())
                    AddOuterVertex(creatorTile, vertex);
        }
    }

    if (creatorBoard->GetBoardShape() == EBoardShape::Hex)
    {
        Log("NO skip");
        return bookkeeper;
    }

    auto cache = bookkeeper.GetOuterVertices();
    for (auto vertex : cache)
    {
        Vertex* next = GetVertex(vertex->Neighbors(), (vertex->index + 1 + size) % size);
        Vertex* last = GetVertex(vertex->Neighbors(), (vertex->index - 1 + size) % size);

        if (cache.Contains(next) && cache.Contains(last))
            bookkeeper.MarkAsSkipped(vertex);
    }
    return bookkeeper;
}
void AntMarchSpawner::TraverseVertices(VertexBookkeeper& bookkeeper, int size) const
{
    int limit = 400;
    while (--limit >= 0 && bookkeeper.OuterVerticesNum() > 0)
    {
        bookkeeper.AddRing();
        
        if (auto vertexTraveler = VertexTraveler::Create(bookkeeper, size))
            vertexTraveler->TraverseVertexRing(bookkeeper);
    }
    
    if (limit < 0)
        Log("infinite outer", RED);
}
TArray<TArray<FVector>> AntMarchSpawner::CenterAntVertices(VertexBookkeeper& bookkeeper) const
{
    TArray<TArray<FVector>> positions;
    for (auto& vertexGroup : bookkeeper.GetAntVertices())
    {
        positions.Add(TArray<FVector>());
        for (auto& vertex : vertexGroup)
        {
            int count = 1;
            auto totalPosition = vertex->GetWorldPosition();
            for (auto neighbor : vertex->neighbors)
            {
                totalPosition += neighbor->GetWorldPosition();
                count++;
            }
            auto averagePosition = totalPosition / count;
            positions.Last().Add(averagePosition);
        }
    }
    return positions;
}

// float ToTriIndex(int index, const ATile* tile) { return index + (static_cast<const FTriCoord*>(tile->GetCoord().Get())->GetIsUp() ? 0.0f : 1); }

/*  Vertex Traveler */
auto AntMarchSpawner::VertexTraveler::Create(const VertexBookkeeper& bookkeeper, int size) -> TUniquePtr<VertexTraveler> {

    // Gets the Upper-Left most vertex
    auto minimumVertex = bookkeeper.GetMinimumOuterVertex();
    
    int outerVertexCount = 0;
    for (auto neighbor : minimumVertex->neighbors)
        if (Cast<ACreatorTile>(neighbor->GetTile())->GetIsSelected())
            outerVertexCount++;
    auto shape = minimumVertex->GetTile()->Shape();
    int indexDistance = shape == EBoardShape::Triangle ? 2 : 1;
    return MakeUnique<VertexTraveler>(minimumVertex, outerVertexCount <= size / 2, size, indexDistance);
}
     AntMarchSpawner::VertexTraveler::VertexTraveler(Vertex* minVertex, bool isOuter, int size, int indexDistance)
        : minVertex{ minVertex }, isOuter{ isOuter }, size{ size }, indexDistance{ indexDistance }
{
    minTile             = minVertex->GetTile();
    internalVertexIndex = startIndex   = minVertex->index;
    currentTile         = exitLoopTile = minTile;
    prevInternalIndex   = (startIndex  + indexDistance) % size;
#ifdef SHOW_DEBUG
    // minTile->SetColor(ETileColor::Yellow, true);
#endif
}
void AntMarchSpawner::VertexTraveler::TraverseVertexRing(VertexBookkeeper& bookkeeper)
{
    int limit = 400;

    int& travelingIndex = isOuter ? internalVertexIndex : externalVertexIndex;
    Vertex*& travelingVertex = isOuter ? internalVertex : externalVertex;

    UE_Log("\n.\n.\nNext Ring, vertex: "_f + internalVertexIndex + ".\n.\n"_f);
    int lastOuterVerticesCount = bookkeeper.GetOuterVertices().Num();

    // While the ring hasn't been finished
    while (--limit >= 0 && (exitVertexIndex == -1 || !(exitLoopTile == minTile && startIndex == exitVertexIndex)))
    {
        Travel(bookkeeper, travelingVertex, travelingIndex);
        UE_Log(PRINT(bookkeeper.GetOuterVertices().Num()));

        if (bookkeeper.GetOuterVertices().Num() == lastOuterVerticesCount)
            return;
        lastOuterVerticesCount = bookkeeper.GetOuterVertices().Num();
    }
    
    if (limit < 0)
        Log("inf inner", RED);
}
bool AntMarchSpawner::VertexTraveler::HasChangedTiles(VertexBookkeeper& bookkeeper)
{
    if (!bookkeeper.IsUnprocessedOuterVertex(externalVertex) || bookkeeper.IsProcessed(externalVertex))
    {
        if (internalVertex && externalVertex)
            UE_Log("internal from: "_f + internalVertex->GetTile()->ID() + " external vert "_f +
                externalVertexIndex + " of tile "_f + externalVertex->GetTile()->ID() + " invalid."_f);
        return false;
    }
 
    Vertex* ex = externalVertex;
    // Check if there is a path from internalVertex to externalVertex comprised of all outerVertices that have been skipped
    // If there isn't, then we should not include this vertex for this island, we'll come back to it later
    for (int i = 1; i < size; i++)
    {
        // TODO:: is this needed
        if (!ex)
            break;
        int neighborIndex = (ex->index - 1 + size) % size;
        ex = GetNeighborFromVertex(internalVertex->GetTile(), internalVertexIndex, neighborIndex);

        if (internalVertex->GetTile()->Board()->GetBoardShape() != EBoardShape::Hex && !bookkeeper.HasBeenSkipped(ex))
        {
            if (ex == internalVertex)
                break;
            UE_Log("external vert"_f + externalVertexIndex + " of tile "_f + externalVertex->GetTile()->ID() + " is ignored; it wasnt orginally skipped."_f);
            return false;
        }
    }

    // Mark vertex as processed if we are able to move onto adjacent tile without turning
    bookkeeper.MarkAsProcessed(internalVertex);
    internalVertexIndex = externalVertexIndex;
    currentTile = externalVertex->GetTile();

    UE_Log("");
    UE_Log("------------------- switched tiles to ---> "_f + currentTile->ID(), " <--- with : ", PRINT(internalVertexIndex));
    UE_Log(PRINT(minTile), PRINT(startIndex));
    UE_Log("");
    return true;
}
void AntMarchSpawner::VertexTraveler::NextInternalVertex(int direction)
{
    prevInternalIndex   = internalVertexIndex;
    internalVertexIndex = (internalVertexIndex + direction + size) % size;
    UE_Log("next internal: ", internalVertexIndex);
}
void AntMarchSpawner::VertexTraveler::Travel(VertexBookkeeper& bookkeeper, Vertex*& travelingVertex, int& travelingIndex)
{
    for (int i = 1; i < size; i++)
    {
        NextExternalVertex(isOuter ? -i : i);
        if (HasChangedTiles(bookkeeper))
            return;
    }
    
    ProcessVertex(bookkeeper, travelingVertex, travelingIndex);
    NextInternalVertex(-indexDistance);
    
    exitLoopTile    = travelingVertex->GetTile();
    exitVertexIndex = travelingIndex;
}
void AntMarchSpawner::VertexTraveler::ProcessVertex(VertexBookkeeper& bookkeeper, const Vertex* vertex, int vertexIndex) const
{
    // If the index has changed, that means we've turned, which makes it an ant vertex
    if (!bookkeeper.IsProcessed(internalVertex) && !bookkeeper.HasBeenSkipped(internalVertex))
    {
        if (internalVertexIndex != prevInternalIndex)
        {
            bookkeeper.MarkAsAntVertex(internalVertex);
            UE_Log("Vertex "_f + internalVertexIndex + " from "_f + internalVertex->GetTile()->ID() + " antified."_f);
        }
        else
        {
            bookkeeper.MarkAsProcessed(internalVertex);
            UE_Log("Vertex "_f + internalVertexIndex + " from "_f + internalVertex->GetTile()->ID() + " processed."_f);
        }
    }

    if (!vertex) { Log("Null vertex"); return; }
}
void AntMarchSpawner::VertexTraveler::NextExternalVertex(int direction)
    // Gets the next vertex from an outside tile (external vertex), based off the current vertex from the current tile (internal vertex)
{
    externalVertexIndex = (internalVertexIndex + direction + size) % size;
    externalVertex      = GetNeighborFromVertex(currentTile, internalVertexIndex, externalVertexIndex);

    if (currentTile == nullptr)
    {
        Log("null current tile ant marcher spawn", RED);
        return;
    }
    TArray<Vertex*> vertices;

    // TODO:: This is possibly the only time currentTile is used for non debugging
    currentTile->MeshGenerator->Vertices().GenerateValueArray(vertices);
    internalVertex = GetVertex(vertices, internalVertexIndex);
}
Vertex* AntMarchSpawner::VertexTraveler::GetNeighborFromVertex(const ATile* tile, int vertexIndex, int neighborIndex) const
{
    TMap<int, Vertex*> vertices;
    auto pivot = tile->MeshGenerator->Vertices()[vertexIndex];
    
    for (auto neighbor : pivot->neighbors)
        vertices.Add(neighbor->index, neighbor);
    vertices.Add(pivot->index, pivot);
    
    if (!vertices.Contains(neighborIndex))
        return nullptr;
    return vertices[neighborIndex];
}
Vertex* AntMarchSpawner::GetVertex(const TArray<Vertex*>& vs, int index)
{
    for (auto vertex : vs)
        if (vertex->index == index)
            return vertex;
    return nullptr;
};

// on travel end
// bookkeeper.MarkAsProcessed(bookkeeper.GetLastAntVertex());

/*  Vertex Bookkeeper */
void AntMarchSpawner::VertexBookkeeper::AddOuterVertex(Vertex* vertex)
{
    outerVertices.Add(vertex);
}
void AntMarchSpawner::VertexBookkeeper::MarkAsSkipped(Vertex* vertex)
{
    outerVertices.Remove(vertex);
    skippedVertices.Add(vertex);
}
void AntMarchSpawner::VertexBookkeeper::MarkAsProcessed(Vertex* vertex)
{
    outerVertices.Remove(vertex);
    processedVertices.Last().Add(vertex);
}
void AntMarchSpawner::VertexBookkeeper::MarkAsAntVertex(Vertex* vertex)
{
    outerVertices.Remove(vertex);

    // for (auto neighbor : vertex->neighbors)
    //     if (IsAntVertex(neighbor))
    //     {
    //         MarkAsProcessed(vertex);
    //         return;
    //     }
    antVertices.Last().Add(vertex);
}
void AntMarchSpawner::VertexBookkeeper::AddRing()
{
    processedVertices.Add(TArray<Vertex*>());
    antVertices.Add(TArray<Vertex*>());
}
void AntMarchSpawner::VertexBookkeeper::Draw()
{
    auto world = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport)->World();

    for (auto vertexGroup : antVertices)
        for (auto vertex : vertexGroup)
            if (vertex)
                DrawDebugSphere(world, vertex->GetWorldPosition(), 2, 8, FColor::Yellow, true, -1, 0, 2.f);
    for (auto vertex : outerVertices)
        if (!IsAntVertex(vertex))
            if (vertex)
                DrawDebugSphere(world, vertex->GetWorldPosition(), 2, 8, FColor::Red, true, -1, 0, 2.f);
    for (auto vertex : skippedVertices)
        if (!IsAntVertex(vertex))
            if (!outerVertices.Contains(vertex))
                if (vertex)
                    DrawDebugSphere(world, vertex->GetWorldPosition(), 2, 8, FColor::Orange, true, -1, 0, 2.f);
    for (auto vertexGroup : processedVertices)
        for (auto vertex : vertexGroup)
            if (!IsAntVertex(vertex))
                if (!outerVertices.Contains(vertex))
                    if (!skippedVertices.Contains(vertex))
                        if (vertex)
                            DrawDebugSphere(world, vertex->GetWorldPosition(), 2, 8, FColor::Blue, true, -1, 0, 2.f);   
}
Vertex* AntMarchSpawner::VertexBookkeeper::GetMinimumOuterVertex() const
{
    auto minVertex = outerVertices[0];
    for (auto vertex : outerVertices)
    {
        if (vertex->Z() >  minVertex->Z()
            || vertex->Z() == minVertex->Z() && vertex->Y() <  minVertex->Y()
            || vertex->Z() == minVertex->Z() && vertex->Y() == minVertex->Y() && vertex->X() < minVertex->X())
            minVertex = vertex;
    }
    return minVertex;
}

//415