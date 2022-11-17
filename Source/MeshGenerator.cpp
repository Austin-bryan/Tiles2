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

    DrawQuad(0, Lengths.X, Lengths.Z, FRotator(0,   0,   0), GetOrigin(FVector::RightVector, Lengths.Y));
    DrawQuad(1, Lengths.X, Lengths.Z, FRotator(0, 180,   0), GetOrigin(FVector::LeftVector, Lengths.Y));
    DrawQuad(2, Lengths.Y, Lengths.Z, FRotator(0, -90,   0), GetOrigin(FVector::ForwardVector, Lengths.X));
    DrawQuad(3, Lengths.Y, Lengths.Z, FRotator(0,  90,   0), GetOrigin(FVector::BackwardVector, Lengths.X));
    DrawQuad(4, Lengths.X, Lengths.Y, FRotator(0,   0, -90), GetOrigin(FVector::UpVector, Lengths.Z));
    DrawQuad(5, Lengths.X, Lengths.Y, FRotator(0,   0,  90), GetOrigin(FVector::DownVector, Lengths.Z));
}

void AMeshGenerator::DrawQuad(const int index, const int width, const int height, const FRotator rotator, const FVector origin)
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
        
        v+= origin;
        v = trans.InverseTransformPosition(v);
        v = rotator.RotateVector(v);
        v = trans.TransformPosition(v);

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
