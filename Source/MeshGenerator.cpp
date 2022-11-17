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

    const auto GetOrigin = [this](const FVector normal, const float length)
    {
        return FVector(length, length, length) * normal;
    };
    
    DrawQuad(FRotator(0, 0, 0), GetOrigin(FVector::RightVector, yLength));
    Mesh->CreateMeshSection(0, vertices, triangles, normals, UV, colors, tangents, true);
    
    DrawQuad(FRotator(0, 180, 0), GetOrigin(FVector::LeftVector, yLength));
    Mesh->CreateMeshSection(1, vertices, triangles, normals, UV, colors, tangents, true);
    
    DrawQuad(FRotator(0, -90, 0), GetOrigin(FVector::ForwardVector, xLength));
    Mesh->CreateMeshSection(2, vertices, triangles, normals, UV, colors, tangents, true);
    
    DrawQuad(FRotator(0, 90, 0), GetOrigin(FVector::BackwardVector, xLength));
    Mesh->CreateMeshSection(3, vertices, triangles, normals, UV, colors, tangents, true);
    
    DrawQuad(FRotator(0, 0, -90), GetOrigin(FVector::UpVector, xLength));
    Mesh->CreateMeshSection(4, vertices, triangles, normals, UV, colors, tangents, true);
    
    DrawQuad(FRotator(0, 0, 90), GetOrigin(FVector::DownVector, xLength));
    Mesh->CreateMeshSection(5, vertices, triangles, normals, UV, colors, tangents, true);
}

void AMeshGenerator::DrawQuad(const FRotator rotator, const FVector origin)
{
    ClearData();
    const FTransform trans { origin };

    for(int x = 0; x <= xLength; x++)
    for(int y = 0; y <= yLength; y++)
    {
        // Centers mesh
        const auto GetVertex = [this](const int n, const float length)
        {
            return n * Size - Size * length / 2;
        };
        auto v =  FVector(GetVertex(x, xLength), 0, GetVertex(y, yLength));
        
        v+= origin;
        v = trans.InverseTransformPosition(v);
        v = rotator.RotateVector(v);
        v = trans.TransformPosition(v);

        // todo:: bug fixes:
        // todo: adding the origin isnt needed
        // todo: rotator doesnt effect the normal
        // todo: figure out what actually causes the origin to be placed
        vertices.Add(v);
        // vertices.Add(origin + normal * FVector(GetVertex(x, xLength), 0, GetVertex(y, yLength)));
        UV.Add(FVector2d(x, y));
    }
    UKismetProceduralMeshLibrary::CreateGridMeshTriangles(xLength + 1, yLength + 1, true, triangles);
    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(vertices, triangles, UV, normals, tangents);
}

void AMeshGenerator::ClearData()
{
    vertices.Empty();
    triangles.Empty();
    UV.Empty();
}
