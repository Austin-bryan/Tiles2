#include "SelectionBox.h"
#include "AssetDir.h"

//todo::replcae defines with fstring constants to avoid having to create locals to deference
ASelectionBox::ASelectionBox()
{
    const auto mat = LoadMaterialFromPath(TEXT(MAT_SELECTION_BOX));
    const auto staticMesh = LoadObjectFromPath<UStaticMesh>(TEXT(SQUARE_TILE));

    mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
    mesh->SetMaterial(0, mat);
    mesh->SetStaticMesh(staticMesh);
    mesh->SetWorldScale3D(FVector(0.001f, 1, 1));
    ASelectionBox::Rename(TEXT("Selection Box"));
    SetVisibility(false);
}
void ASelectionBox::BeginPlay()
{
    Super::BeginPlay();
}

void ASelectionBox::SetVisibility(const bool visibility)
{
    isVisible = visibility;
    SetActorScale3D(FVector::Zero());
    mesh->SetVisibility(visibility);
}

