#include "MeshGenerator.h"

#include "KismetProceduralMeshLibrary.h"
#include "Logger.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Vertex.h"

/*
     2  1
   3      0
     4  5
 */

TArray<Vertex> UMeshGenerator::UniversalVertices;
float UMeshGenerator::distance;

UMeshGenerator::UMeshGenerator()
{
    Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
    Mesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
}
void UMeshGenerator::BeginPlay()
{
    Super::BeginPlay();
    ClearData();

    if (MeshDistance)
        distance = MeshDistance;
    const auto GetOrigin = [this](const FVector normal, float length)
    {
        length = Size / 2 * length;
        return FVector(length, length, length) * normal;
    };
    DrawHex(0, FRotator::ZeroRotator, GetOrigin(FVector::RightVector, Lengths.Y));

    return;
    DrawQuad(0, Lengths.X, Lengths.Z, FRotator(0, 0, 0),   GetOrigin(FVector::RightVector, Lengths.Y));       // Front Quad
    DrawQuad(1, Lengths.X, Lengths.Z, FRotator(0, 180, 0), GetOrigin(FVector::LeftVector, Lengths.Y));        // Back Quad
    DrawQuad(2, Lengths.Y, Lengths.Z, FRotator(0, -90, 0), GetOrigin(FVector::ForwardVector, Lengths.X));     // Right Quad
    DrawQuad(3, Lengths.Y, Lengths.Z, FRotator(0, 90, 0),  GetOrigin(FVector::BackwardVector, Lengths.X));    // Left Quad
    DrawQuad(4, Lengths.X, Lengths.Y, FRotator(0, 0, -90), GetOrigin(FVector::UpVector, Lengths.Z));          // Top Quad
    DrawQuad(5, Lengths.X, Lengths.Y, FRotator(0, 0, 90),  GetOrigin(FVector::DownVector, Lengths.Z));        // Bottom Quad
}
#ifdef SHOW_VERTICES
void UMeshGenerator::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    for (int i = 0; i < vertices.Num(); i++)
        DrawDebugSphere(GetWorld(), vertices[i], 2, 64, FColor::White);
}
#endif

void UMeshGenerator::Merge()
{
    TArray<Vertex*> neighbors;

    for (auto& vertexA : UniversalVertices)
    {
        if (vertexA.IsMerged())
            continue;
        neighbors.Empty();

        int count = 0;
        FVector sum = FVector::Zero();
        for (auto& vertexB : UniversalVertices)
        {
            if (!vertexB.IsMerged()
              && FVector::Distance(vertexA.GetPosition(), vertexB.GetPosition()) <= distance)
            {
                neighbors.Add(&vertexB);
                sum += vertexB.GetPosition();
                count++;
            }
        }
        if (neighbors.Num() < 2)
            continue;
        for (const auto& neighbor : neighbors)
        {
            const FVector average = sum / count;
            neighbor->SetPosition(average);
        }
    }
}
void UMeshGenerator::UpdateMesh(const int index)
{
    UKismetProceduralMeshLibrary::CreateGridMeshTriangles(Lengths.X + 1, Lengths.X + 1, false, triangles);
    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(vertices, triangles, UV, normals, tangents);
    Mesh->CreateMeshSection(index, vertices, triangles, normals, UV, colors, tangents, true);
}

void UMeshGenerator::DrawHex(const int index, const FRotator faceAngle, const FVector origin)
{
    ClearData();
    const double radius = Lengths.X * Size;

    for (int i = 0; i < 6; i++)
    {
        vertices.Add(FVector(radius * UKismetMathLibrary::DegCos(i * 60), 0, radius * UKismetMathLibrary::DegSin(i * 60)));
        vertices[i] += GetOwner()->GetActorLocation();
        UniversalVertices.Add(Vertex(i, vertices[i], this));
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

        vertices.Add(v);
        UV.Add(FVector2d(w, h));
    }
    UKismetProceduralMeshLibrary::CreateGridMeshTriangles(width + 1, height + 1, true, triangles);
    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(vertices, triangles, UV, normals, tangents);
    Mesh->CreateMeshSection(index, vertices, triangles, normals, UV, colors, tangents, true);
}
void UMeshGenerator::ClearData()
{
    vertices.Empty();
    triangles.Empty();
    UV.Empty();
}
