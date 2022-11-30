// ReSharper disable CppParameterMayBeConst
// ReSharper disable CppLocalVariableMayBeConst
#include "MeshGenerator.h"

#include "Logger.h"
#include "Board.h"
#include "Vertex.h"
#include "Tile.h"
#include "ActiveSocket.h"
#include "CreatorTile.h"
#include "CreatorBoard.h"
#include "KismetProceduralMeshLibrary.h"
#include "VectorTypes.h"
#include "Kismet/KismetMathLibrary.h"

/*
     2  1
   3      0
     4  5
 */

TArray<Vertex> UMeshGenerator::UniversalVertices;
TArray<UMeshGenerator*> UMeshGenerator::Generators;
TArray<ACreatorTile*> UMeshGenerator::TilesToMerge;

UMeshGenerator::UMeshGenerator() { }

#ifdef DRAW_DEBUG
void UMeshGenerator::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    for (int i = 0; i < vertices.Num(); i++)
        DrawDebugSphere(GetWorld(), vertices[i]->GetWorldPosition(), 1, 4, FColor::White);
}
#endif
bool LineLineIntersection(FVector startA, FVector endA, FVector startB, FVector endB, FVector& out_intersection)
{
    // Line AB represented as a1x + b1y = c1
    double a1 = endA.Z - startA.Z;
    double b1 = startA.X - endA.X;
    double c1 = a1 * (startA.X) + b1 * (startA.Z);
 
    // Line CD represented as a2x + b2y = c2
    double a2 = endB.Z - startB.Z;
    double b2 = startB.X - endB.X;
    double c2 = a2 * (startB.X) + b2 * (startB.Z);
 
    double determinant = a1 * b2 - a2 * b1;

    // Lines are parallel. Only occurs in squares, so we can just average the vertices
    if (FMath::Abs(determinant) < 0.01f)
    {
        out_intersection = (startA + startB) / 2;
        return true;
    }
    double x = (b2 * c1 - b1 * c2) / determinant;
    double z = (a1 * c2 - a2 * c1) / determinant;

    out_intersection = FVector(x, 0, z);
    return true;
}

void UMeshGenerator::Merge()
{
    Generators.Empty();
    TArray<Vertex*> queuedVertices;

    for (const auto& creatorTileA : TilesToMerge)
        MergeWithNeighbors(queuedVertices, creatorTileA);
    for (auto& vertex : queuedVertices)
        vertex->ApplyPosition();
    for (const auto& generator : Generators)
        generator->Draw();
}
void UMeshGenerator::MergeWithNeighbors(TArray<Vertex*>& queuedVertices, ACreatorTile* const& creatorTileA)
{
    for (auto vertexA : creatorTileA->MeshGenerator->vertices)
    {
        int selectionCount = 0;
        const auto GetCreatorTile = [](const Vertex* vertex) { return Cast<ACreatorTile>(vertex->GetTile()); };

        if (!GetCreatorTile(vertexA)->GetIsSelected())
            continue;
        
        selectionCount++;
        for (auto vertexB : vertexA->neighbors)
        {
            const auto creatorTileB = GetCreatorTile(vertexB);
            
            if (creatorTileB->GetIsSelected())
                selectionCount++;
        }

        if (selectionCount == 2)
        {
            if (!vertexA->IsMerged())
                for (const auto& creatorTileB : TilesToMerge)
                    if (creatorTileB != creatorTileA)
                        MergeWithNeighbor(queuedVertices, creatorTileA, vertexA, creatorTileB);
        }
        else if (selectionCount > 2)
        {
            FVector sum = FVector::Zero();
            TArray<Vertex*> selectedVertices;
            
            for (auto vertexB : vertexA->neighbors)
            {
                if (!GetCreatorTile(vertexB)->GetIsSelected())
                    continue;
                selectedVertices.Add(vertexB);
                sum += vertexB->GetWorldPosition();
            }
            AverageVertices(selectedVertices, sum);
        }
    }
}

void UMeshGenerator::Init(int _radius, int _vertexCount, int _angleOffset, int _angle)
{
    radius = _radius, vertexCount = _vertexCount, angleOffset = _angleOffset, angle = _angle;
    ClearData();
    Draw();
}
void UMeshGenerator::MergeWithNeighbor(TArray<Vertex*>& queuedVertices, ACreatorTile* const& creatorTileA, Vertex* vertexA, ACreatorTile* const& creatorTileB)
{
    TArray<Vertex*> neighbors;
    neighbors.Add(vertexA);

    for (auto& vertexB : creatorTileB->MeshGenerator->vertices)
    {
        if (!ShouldMergeVertices(vertexA, vertexB))
            continue;
        neighbors.Add(vertexB);

        FVector intersection;
        if (IsIntersectionValid(Cast<ACreatorBoard>(creatorTileA->Board())->VertexMode, creatorTileA, creatorTileB, vertexA, vertexB, intersection))
            QueueVertices(queuedVertices, creatorTileA, vertexA, vertexB, intersection);
    }
}
bool UMeshGenerator::ShouldMergeVertices(const Vertex* vertexA, const Vertex* vertexB)
{
    if (vertexA->IsMerged() || vertexB->IsMerged())
        return false;
    return vertexA->neighbors.Contains(vertexB);
}
void UMeshGenerator::QueueVertices(TArray<Vertex*>& queuedVertices, ACreatorTile* const& creatorTileA, Vertex* vertexA, Vertex* vertexB, FVector intersection)
{
#ifdef DRAW_DEBUG
    DrawDebugSphere(creatorTileA->GetWorld(), intersection, 5, 16, FColor::Green, true);
#endif
    vertexA->QueuePosition(intersection);
    vertexB->QueuePosition(intersection);
    queuedVertices.Add(vertexA);
    queuedVertices.Add(vertexB);
}
bool UMeshGenerator::IsIntersectionValid(EVertexMode vertexMode, const ACreatorTile* const creatorTileA, const ACreatorTile* const creatorTileB, const Vertex* vertexA, const Vertex* vertexB, FVector& intersection)
{
    // There are two vertexes, prev & next, that can be used as the starting FVector for the intersection line
    // The vertex thats closest to the tile is also being merged, so we want to avoid it
    // This function seeks the vertex that is furtherst away from the tile
    const double location1 = FVector::Distance(creatorTileB->GetActorLocation(), vertexA->NextVertex()->GetWorldPosition());
    const double location2 = FVector::Distance(creatorTileB->GetActorLocation(), vertexA->PrevVertex()->GetWorldPosition());
    const double location3 = FVector::Distance(creatorTileA->GetActorLocation(), vertexB->NextVertex()->GetWorldPosition());
    const double location4 = FVector::Distance(creatorTileA->GetActorLocation(), vertexB->PrevVertex()->GetWorldPosition());
    
    Vertex* vA = location1 >= location2 ? vertexA->NextVertex() : vertexA->PrevVertex();
    Vertex* vB = location3 >= location4 ? vertexB->NextVertex() : vertexB->PrevVertex();

    return GetIntersection(creatorTileA->GetWorld(), vA, vertexA, vB, vertexB, intersection);
}
bool UMeshGenerator::GetIntersection(const UWorld* worldContext, const Vertex* startA, const Vertex* endA, const Vertex* startB, const Vertex* endB, FVector& intersection)
{
    const FVector lineAStart = startA->GetWorldPosition(), lineAEnd = GetEndVertex(startA, endA);
    const FVector lineBStart = startB->GetWorldPosition(), lineBEnd = GetEndVertex(startB, endB);

#ifdef DRAW_DEBUG
    DrawDebugLine(worldContext, lineAStart, lineAEnd, FColor::Green, true, 100, 100);
    DrawDebugLine(worldContext, lineBStart, lineBEnd, FColor::White, true, 100, 100);
#endif
    bool result = LineLineIntersection(lineAStart, lineAEnd, lineBStart, lineBEnd, intersection);

    if (intersection.Length() > 1000)
        return false;
    return result;
}
void UMeshGenerator::AverageVertices(TArray<Vertex*> neighbors, FVector sum)
{
    for (const auto& neighbor : neighbors)
    {
        FVector average = sum / neighbors.Num();
        average.Y = 0;
        neighbor->SetPosition(average);
    }
}
FVector UMeshGenerator::GetEndVertex(const Vertex* start, const Vertex* end)
{
    // This function isn't technically needed, as the intersection uses lines, not line segments,
    // but it makes the visualization easier
    const FVector startPos = start->GetWorldPosition();
    const FTransform startTrans{ startPos };
    FVector transformed = startTrans.InverseTransformPosition(end->GetWorldPosition());
    transformed.Normalize();

    return end->GetWorldPosition() + transformed * 20;
}

void UMeshGenerator::UpdateMesh()
{
    UKismetProceduralMeshLibrary::CreateGridMeshTriangles(roundedVertices.Num(), roundedVertices.Num(), false, triangles);
    ProceduralMesh->CreateMeshSection(0, roundedVertices, triangles, normals, UV, colors, tangents, true);
}
void UMeshGenerator::Draw()
{
    ClearData();

    for (int i = 0; i < vertexCount; i++)
        vertices.Add(new Vertex(i, vertexCount, FVector(radius * UKismetMathLibrary::DegCos(i * angle + angleOffset), 0
                                                      , radius * UKismetMathLibrary::DegSin(i * angle + angleOffset)), this));

    // TODO:: have angle be a function of 360 / vertexCount
    const float circleRadius = 25.0f;

    TArray<FVector> circleOrigins;
    for (const auto& vertex : vertices)
        circleOrigins.Add(-vertex->GetLocalPosition().GetSafeNormal() * circleRadius * FMath::Sqrt(2.0f) + vertex->GetLocalPosition());

    roundedVertices.Empty();
    for (int i = 0; i < vertexCount; i++)
    {
        if (vertices[i]->IsMerged())
        {
            roundedVertices.Add(vertices[i]->GetLocalPosition());
            continue;
        }
        FVector circleOrigin = circleOrigins[i];

        int curveCount = 5;
        for (int j = -curveCount; j <= curveCount; j++)
        {
            FTransform circleTrans{ circleOrigin };
            FVector x = (circleOrigin.GetSafeNormal() * circleRadius + circleOrigin);
            
            x = circleTrans.InverseTransformPosition(x);
            x = FRotator(j * 45 / curveCount, 0, 0).RotateVector(x);
            x = circleTrans.TransformPosition(x);
            
            roundedVertices.Add(x);
        }
    }
    UpdateMesh();
}
void UMeshGenerator::ClearData()
{
    // vertexPositions.Empty();
    triangles.Empty();
    UV.Empty();
}