#include "SelectionBox.h"
#include "AssetDir.h"
#include "CreatorTile.h"
#include "Components/BoxComponent.h"

//TODO:: replcae defines with fstring constants to avoid having to create locals to deference
//TODO:: allow for shift drag
//TODO:: prevent selection flip flop, perhaps by having a seperate collider just for deselction
//TODO:: prevent last time from being selected when exiting drag
//TODO:: glowing selection background
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
    collider->SetRelativeLocation(FVector(-550, 0, 0));

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
    
    if (isVisible)
        ScaleArea(0, 0);
    mesh->SetVisibility(visibility);
}
void ASelectionBox::ScaleArea(float width, float height)
{
    width  /= 100;
    height /= 100;
    constexpr float expansion = 0.125f;
    
    mesh->SetWorldScale3D(FVector(0.001f, width, height));
    collider->SetWorldScale3D(FVector(1, width + expansion, height + expansion));
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

