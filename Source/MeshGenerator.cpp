#include "MeshGenerator.h"

AMeshGenerator::AMeshGenerator()
{
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
    Mesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

 
}

void AMeshGenerator::BeginPlay()
{
    const float size = Size;
    Super::BeginPlay();
    const TArray<FVector> vertices =
    {
        { -size, 0,  size },
        {  size * 3, 0,  size },
        {  size * 2, 0, -size },
        { -size, 0, -size },
    };
    const TArray faces = { 0, 1, 2, 3 };
    const TArray<FVector> normals;
    const TArray<FVector2D> UV =
    {
        { -1,  1 },
        {  1,  1 },
        {  1, -1 },
        { -1, -1 },
    };
    const TArray<FColor> colors;
    const TArray<FProcMeshTangent> tangents;

    

    Mesh->CreateMeshSection(0, vertices, faces, normals, UV, colors, tangents, true);
}
