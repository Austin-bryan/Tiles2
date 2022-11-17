#include "MeshGenerator.h"
#include "KismetProceduralMeshLibrary.h"

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
    DrawQuad();
    
    Mesh->CreateMeshSection(0, vertices, triangles, normals, UV, colors, tangents, true);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AMeshGenerator::DrawQuad()
{
    for(int x = 0; x <= xLength; x++)
    for(int y = 0; y <= yLength; y++)
    {
        vertices.Add(FVector(x * Size, 0, y * Size));
        UV.Add(FVector2d(x / xLength, y / yLength));
    }

    UKismetProceduralMeshLibrary::CreateGridMeshTriangles(xLength + 1, yLength + 1, true, triangles);
    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(vertices, triangles, UV, normals, tangents);
}
