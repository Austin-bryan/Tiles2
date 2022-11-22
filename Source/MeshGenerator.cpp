// ReSharper disable CppParameterMayBeConst
// ReSharper disable CppLocalVariableMayBeConst
#include "MeshGenerator.h"

#include "CreatorTile.h"
#include "KismetProceduralMeshLibrary.h"
#include "Logger.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Vertex.h"
#include "Tile.h"

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
        DrawDebugSphere(GetWorld(), vertices[i].GetWorldPosition(), 2, 16, FColor::White);
}
#endif

bool lineLineIntersection(FVector A, FVector B, FVector C, FVector D, FVector& out_intersection)
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
    int genCount = 0;
    Generators.Empty();

    int loop = 0;
    int distanceCheck = 0;
    int test = 0;

    TArray<Vertex*> queuedVertices;

    for (const auto& creatorTileA : TilesToMerge)
    for (auto& vertexA : creatorTileA->MeshGenerator->vertices)
    {
        loop++;
        if (vertexA.IsMerged())
            continue;
        neighbors.Empty();

        int count = 0;
        FVector sum = FVector::Zero();

        for (const auto& creatorTileB : TilesToMerge)
        {
            if (creatorTileB == creatorTileA)
                continue;
            for (auto& vertexB : creatorTileB->MeshGenerator->vertices)
            {
                if (!vertexB.IsMerged() && FVector::Distance(vertexA.GetWorldPosition(), vertexB.GetWorldPosition()) <= distance)
                {
                    neighbors.Add(&vertexB);
                    sum += vertexB.GetWorldPosition();
                    count++;

                    FVector intersection;
                    
                    const auto GetEnd = [](const Vertex start, const Vertex end)
                    {
                        const FVector startPos = start.GetWorldPosition();
                        const FTransform startTrans{ startPos };
                        FVector transformed = startTrans.InverseTransformPosition(end.GetWorldPosition());
                        transformed.Normalize();
                        return end.GetWorldPosition() + transformed * 20;
                    };
                    const auto foo = [GetEnd, &intersection](const UWorld* worldContext, const Vertex startA, const Vertex endA, const Vertex startB, const Vertex endB)
                    {
                        const FVector lineAStart = startA.GetWorldPosition(), lineAEnd = GetEnd(startA, endA);
                        const FVector lineBStart = startB.GetWorldPosition(), lineBEnd = GetEnd(startB, endB);

#ifdef DRAW_DEBUG
                        DrawDebugLine(worldContext, lineAStart, lineAEnd, FColor::Yellow, true, 100, 100);
                        DrawDebugLine(worldContext, lineBStart, lineBEnd, FColor::White, true, 100, 100);
#endif
                        bool result = lineLineIntersection(lineBStart, lineBEnd, lineAStart, lineAEnd, intersection);
                        if (intersection.Length() > 1000)
                            return false;
                        return result;
                    };
                    neighbors.Add(&vertexA);

                    if (neighbors.Num() != 2)
                        continue;
                    if (foo(creatorTileA->GetWorld(), vertexA.NextVertex(), vertexA, vertexB.PrevVertex(), vertexB))
                    {
#ifdef DRAW_DEBUG
                        DrawDebugSphere(creatorTileA->GetWorld(), intersection, 5, 64, FColor::Red, true);
#endif
                        vertexA.QueuePosition(intersection);
                        vertexB.QueuePosition(intersection);
                        queuedVertices.Add(&vertexA);
                        queuedVertices.Add(&vertexB);
                    }
                }
            }
        }

        if (neighbors.Num() <= 2)
            continue;
        for (const auto& neighbor : neighbors)
        {
            FVector average = sum / count;
            average.Y = 0;
            neighbor->SetPosition(average);
        }
    }
    for (auto& vertex : queuedVertices)
        vertex->ApplyPosition();
    for (const auto& generator : Generators)
        generator->UpdateMesh(0);
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