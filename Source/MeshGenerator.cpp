#include "MeshGenerator.h"
#include "KismetProceduralMeshLibrary.h"
#include "Logger.h"

AMeshGenerator::AMeshGenerator()
{
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
    Mesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

 
}

void AMeshGenerator::BeginPlay()
{
    Super::BeginPlay();
    ClearData();

    const auto GetOrigin = [this](const FVector normal, float length)
    {
        length = Size / 2 * length;
        return FVector(length, length, length) * normal;
    };
    DrawHex(0, FRotator::ZeroRotator, GetOrigin(FVector::RightVector, Lengths.Y));

    if (!Other)
        return;

    

    return;
    DrawQuad(0, Lengths.X, Lengths.Z, FRotator(0, 0, 0), GetOrigin(FVector::RightVector, Lengths.Y));       // Front Quad
    
    DrawQuad(1, Lengths.X, Lengths.Z, FRotator(0, 180, 0), GetOrigin(FVector::LeftVector, Lengths.Y));      // Back Quad
    DrawQuad(2, Lengths.Y, Lengths.Z, FRotator(0, -90, 0), GetOrigin(FVector::ForwardVector, Lengths.X));   // Right Quad
    DrawQuad(3, Lengths.Y, Lengths.Z, FRotator(0, 90, 0), GetOrigin(FVector::BackwardVector, Lengths.X));   // Left Quad
    DrawQuad(4, Lengths.X, Lengths.Y, FRotator(0, 0, -90), GetOrigin(FVector::UpVector, Lengths.Z));        // Top Quad
    DrawQuad(5, Lengths.X, Lengths.Y, FRotator(0, 0, 90), GetOrigin(FVector::DownVector, Lengths.Z));       // Bottom Quad
}
void AMeshGenerator::DrawHex(const int index, const FRotator faceAngle, const FVector origin)
{
    ClearData();
    const double radius = Lengths.X * Size;
    const FTransform trans{ origin };

    for (int i = 0; i < 6; i++)
    {
        auto v = FVector(radius * cos(i * 1.0472), 0, radius * sin(i * 1.0472));

        v += origin;
        v  = trans.InverseTransformPosition(v);
        v  = FRotator(0, 180, 0).RotateVector(v);
        v  = trans.TransformPosition(v);

        vertices.Add(v);
        UV.Add(FVector2d(v.X, v.Z));
    }

    UKismetProceduralMeshLibrary::CreateGridMeshTriangles(Lengths.X + 1, Lengths.X + 1, true, triangles);
    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(vertices, triangles, UV, normals, tangents);
    Mesh->CreateMeshSection(index, vertices, triangles, normals, UV, colors, tangents, true);
}
void AMeshGenerator::DrawQuad(const int index, const int width, const int height, const FRotator faceAngle, const FVector origin)
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

void AMeshGenerator::ClearData()
{
    vertices.Empty();
    triangles.Empty();
    UV.Empty();
}
