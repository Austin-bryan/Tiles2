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
const float UMeshGenerator::distance = 10.0f;

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
#ifdef SHOW_VERTICES
void UMeshGenerator::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    for (int i = 0; i < vertices.Num(); i++)
        DrawDebugSphere(GetWorld(), vertices[i] + GetOwner()->GetActorLocation(), 2, 64, FColor::White);
}
#endif

void UMeshGenerator::Merge()
{
    TArray<Vertex*> neighbors;
    int genCount = 0;
    Generators.Empty();

    int loop = 0;
    int distanceCheck = 0;

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
            Log(creatorTileB->ID(), creatorTileB->GetIsSelected());

            FString text;
            if (!creatorTileB->GetIsSelected())
            {
                Log(creatorTileB->ID(), creatorTileB->GetIsSelected());
                continue;
            }
            for (auto& vertexB : creatorTileB->MeshGenerator->vertices)
            {
                distanceCheck++;
                if (!vertexB.IsMerged()
                  && FVector::Distance(vertexA.GetWorldPosition(), vertexB.GetWorldPosition()) <= distance)
                {
                    neighbors.Add(&vertexB);
                    sum += vertexB.GetWorldPosition();
                    count++;
                }
            }
        }
        if (neighbors.Num() < 2)
            continue;
        for (const auto& neighbor : neighbors)
        {
            FVector average = sum / count;
            average.Y = 0;
            neighbor->SetPosition(average);
        }
    }
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