#include "SelectionBox.h"
#include "AssetDir.h"
#include "CreatorTile.h"
#include "Coord.h"
#include "Components/BoxComponent.h"

//todo::replcae defines with fstring constants to avoid having to create locals to deference
ASelectionBox::ASelectionBox()
{
    const auto mat = LoadMaterialFromPath(TEXT(MAT_SELECTION_BOX));
    const auto staticMesh = LoadObjectFromPath<UStaticMesh>(TEXT(SIMPLE_SQUARE));

    root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
    RootComponent = root;

    mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
    mesh->SetMaterial(0, mat);
    mesh->SetStaticMesh(staticMesh);
    mesh->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);

    collider = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collider"));
    collider->SetStaticMesh(staticMesh);
    collider->AttachToComponent(root, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
    collider->SetVisibility(false);
    collider->SetRelativeLocation(FVector(-20, 0, 0));

    ScaleArea(1, 1);
    SetVisibility(false);
}
void ASelectionBox::BeginPlay()
{
    Super::BeginPlay();
    collider->OnComponentBeginOverlap.AddDynamic(this, &ASelectionBox::OnBeginOverlap);
    collider->OnComponentEndOverlap.AddDynamic(this, &ASelectionBox::OnEndOverlap);
}
void ASelectionBox::SetVisibility(const bool visibility)
{
    isVisible = visibility;
    ScaleArea(0, 0);
    mesh->SetVisibility(visibility);
}
void ASelectionBox::ScaleArea(float width, float height)
{
    width  /= 100;
    height /= 100;
    
    mesh->SetWorldScale3D(FVector(0.001f, width, height));
    collider->SetWorldScale3D(FVector(1, width, height));
}
void ASelectionBox::OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp
    , int otherBodyIndex, bool fromSweep, const FHitResult& sweepResult)
{
    Select(otherActor, true);
}

void ASelectionBox::OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp
    , int otherBodyIndex)
{
    Select(otherActor, false);
}

void ASelectionBox::Select(AActor* otherActor, const bool isSelected)
{
    ACreatorTile* creatorTile = Cast<ACreatorTile>(otherActor);

    if (!creatorTile)
        return;
    creatorTile->Select(isSelected, true);    
}

