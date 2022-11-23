// ReSharper disable CppParameterMayBeConst
// ReSharper disable CppLocalVariableMayBeConst
#include "MeshGenerator.h"

#include "Board.h"
#include "CreatorTile.h"
#include "KismetProceduralMeshLibrary.h"
#include "Logger.h"
#include "Kismet/KismetMathLibrary.h"
#include "Vertex.h"
#include "Tile.h"
#include "CreatorBoard.h"

/*
     2  1
   3      0
     4  5
 */

TArray<Vertex> UMeshGenerator::UniversalVertices;
TArray<UMeshGenerator*> UMeshGenerator::Generators;
TArray<ACreatorTile*> UMeshGenerator::TilesToMerge;
const float UMeshGenerator::distance = 10;

UMeshGenerator::UMeshGenerator() { }
void UMeshGenerator::BeginPlay()
{
    Super::BeginPlay();
    ClearData();

    const auto GetOrigin = [this](const FVector normal, float length)
    {
        length = Size / 2 * length;
        return FVector(length, length, length) * normal;
    };
    DrawHex(0, FRotator::ZeroRotator, GetOrigin(FVector::RightVector, Lengths.Y));
}
#ifdef DRAW_DEBUG
void UMeshGenerator::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    for (int i = 0; i < vertices.Num(); i++)
        DrawDebugSphere(GetWorld(), vertices[i].GetWorldPosition(), 1, 16, FColor::White);
}
#endif

bool LineLineIntersection(FVector A, FVector B, FVector C, FVector D, FVector& out_intersection)
{
    // Line AB represented as a1x + b1y = c1
    double a1 = B.Z - A.Z;
    double b1 = A.X - B.X;
    double c1 = a1 * (A.X) + b1 * (A.Z);
 
    // Line CD represented as a2x + b2y = c2
    double a2 = D.Z - C.Z;
    double b2 = C.X - D.X;
    double c2 = a2 * (C.X) + b2 * (C.Z);
 
    double determinant = a1 * b2 - a2 * b1;
 
    if (determinant == 0)
    {
        // The lines are parallel. This is simplified
        // by returning a pair of FLT_MAX
        return false;
    }
    
    double x = (b2 * c1 - b1 * c2) / determinant;
    double z = (a1 * c2 - a2 * c1) / determinant;

    out_intersection = FVector(x, 0, z);
    Log("intersection: ", out_intersection);
    return true;
}

void UMeshGenerator::Merge()
{
    TArray<Vertex*> neighbors;
    Generators.Empty();
    TArray<Vertex*> queuedVertices;

    for (const auto& creatorTileA : TilesToMerge)
        MergeWithNeighbors(neighbors, queuedVertices, creatorTileA);
    for (auto& vertex : queuedVertices)
        vertex->ApplyPosition();
    for (const auto& generator : Generators)
        generator->UpdateMesh(0);
}
void UMeshGenerator::MergeWithNeighbors(TArray<Vertex*>& neighbors, TArray<Vertex*>& queuedVertices, ACreatorTile* const& creatorTileA)
{
    for (auto& vertexA : creatorTileA->MeshGenerator->vertices)
    {
        if (vertexA.IsMerged())
            continue;
        neighbors.Empty();
        FVector sum = FVector::Zero();

        for (const auto& creatorTileB : TilesToMerge)
            if (creatorTileB != creatorTileA)
                MergeWithNeighbor(neighbors, queuedVertices, creatorTileA, vertexA, sum, creatorTileB);
        if (neighbors.Num() <= 2)
            continue;
        AverageVertices(neighbors, sum);
    }
}
void UMeshGenerator::MergeWithNeighbor(TArray<Vertex*>& neighbors, TArray<Vertex*>& queuedVertices, ACreatorTile* const& creatorTileA, Vertex& vertexA, FVector& sum, ACreatorTile* const& creatorTileB)
{
    neighbors.Add(&vertexA);

    for (auto& vertexB : creatorTileB->MeshGenerator->vertices)
    {
        if (!ShouldMergeVertices(vertexA, vertexB))
            continue;
        neighbors.Add(&vertexB);
        sum += vertexB.GetWorldPosition();

        FVector intersection;

        if (neighbors.Num() != 2)
            continue;
        if (IsIntersectionValid(Cast<ACreatorBoard>(creatorTileA->Board())->VertexMode, creatorTileA, vertexA, vertexB, intersection))
            QueueVertices(queuedVertices, creatorTileA, vertexA, vertexB, intersection);
    }
}
bool UMeshGenerator::ShouldMergeVertices(const Vertex& vertexA, const Vertex& vertexB)
{
    return !vertexA.IsMerged() && !vertexB.IsMerged() && FVector::Distance(vertexA.GetWorldPosition(), vertexB.GetWorldPosition()) <= distance;
}
void UMeshGenerator::QueueVertices(TArray<Vertex*>& queuedVertices, ACreatorTile* const& creatorTileA, Vertex& vertexA, Vertex& vertexB, FVector intersection)
{
#ifdef DRAW_DEBUG
    DrawDebugSphere(creatorTileA->GetWorld(), intersection, 2, 64, FColor::Red, true);
#endif
    vertexA.QueuePosition(intersection);
    vertexB.QueuePosition(intersection);
    queuedVertices.Add(&vertexA);
    queuedVertices.Add(&vertexB);
}
bool UMeshGenerator::IsIntersectionValid(EVertexMode vertexMode, const ACreatorTile* const creatorTileA, const Vertex& vertexA, const Vertex& vertexB, FVector& intersection)
{
    return vertexMode == EVertexMode::NextNext && GetIntersection(creatorTileA->GetWorld(), vertexA.NextVertex(), vertexA, vertexB.NextVertex(), vertexB, intersection)
        || vertexMode == EVertexMode::NextPrev && GetIntersection(creatorTileA->GetWorld(), vertexA.NextVertex(), vertexA, vertexB.PrevVertex(), vertexB, intersection)
        || vertexMode == EVertexMode::PrevNext && GetIntersection(creatorTileA->GetWorld(), vertexA.PrevVertex(), vertexA, vertexB.NextVertex(), vertexB, intersection)
        || vertexMode == EVertexMode::PrevPrev && GetIntersection(creatorTileA->GetWorld(), vertexA.PrevVertex(), vertexA, vertexB.PrevVertex(), vertexB, intersection);
}
bool UMeshGenerator::GetIntersection(UWorld* worldContext, Vertex startA, Vertex endA, Vertex startB, Vertex endB, FVector& intersection)
{
    const FVector lineAStart = startA.GetWorldPosition(), lineAEnd = GetEndVertex(startA, endA);
    const FVector lineBStart = startB.GetWorldPosition(), lineBEnd = GetEndVertex(startB, endB);

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
FVector UMeshGenerator::GetEndVertex(Vertex start, Vertex end)
{
    const FVector startPos = start.GetWorldPosition();
    const FTransform startTrans{ startPos };
    FVector transformed = startTrans.InverseTransformPosition(end.GetWorldPosition());
    transformed.Normalize();
        
    return end.GetWorldPosition() + transformed * 20;
}

void UMeshGenerator::UpdateMesh(const int index)
{
    UKismetProceduralMeshLibrary::CreateGridMeshTriangles(Lengths.X + 1, Lengths.X + 1, false, triangles);
    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(vertexPositions, triangles, UV, normals, tangents);
    ProceduralMesh->CreateMeshSection(index, vertexPositions, triangles, normals, UV, colors, tangents, true);
}
void UMeshGenerator::DrawHex(const int index, const FRotator faceAngle, const FVector origin)
{
    ClearData();
    const double radius = Lengths.X * Size;

    for (int i = 0; i < 6; i++)
    {
        vertexPositions.Add(FVector(radius * UKismetMathLibrary::DegCos(i * 60), 0, radius * UKismetMathLibrary::DegSin(i * 60)));
        vertices.Add(Vertex(i, vertexPositions[i], this));
    }
    UpdateMesh(index);
}
void UMeshGenerator::DrawQuad(const int index, const int width, const int height, const FRotator faceAngle, const FVector origin)
{
    ClearData();
    const FTransform trans { origin };

    for(int w = 0; w <= width; w++)
    for(int h = 0; h <= height; h++)
    {
        // Centers mesh
        const auto GetVertex = [this](const int n, const float length)
        {
            return n * Size - Size * length / 2;
        };
        auto v =  FVector(GetVertex(w, width), 0, GetVertex(h, height));
        
        v += origin;
        v  = trans.InverseTransformPosition(v);
        v  = faceAngle.RotateVector(v);
        v  = trans.TransformPosition(v);

        vertexPositions.Add(v);
        UV.Add(FVector2d(w, h));
    }
    UKismetProceduralMeshLibrary::CreateGridMeshTriangles(width + 1, height + 1, true, triangles);
    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(vertexPositions, triangles, UV, normals, tangents);
    ProceduralMesh->CreateMeshSection(index, vertexPositions, triangles, normals, UV, colors, tangents, true);
}
void UMeshGenerator::ClearData()
{
    vertexPositions.Empty();
    triangles.Empty();
    UV.Empty();
}