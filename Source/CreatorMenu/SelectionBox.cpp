#include "SelectionBox.h"
#include "AssetDir.h"
#include "CreatorTile.h"
#include "Components/BoxComponent.h"

//TODO:: allow for shift drag
//TODO:: prevent last time from being selected when exiting drag
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
    collider->SetRelativeLocation(FVector(-580, 0, 0));
    collider->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
    collider->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

    ScaleArea(1, 1);
    SetVisibility(false);
}
void ASelectionBox::BeginPlay()
{
    Super::BeginPlay();
    collider->OnComponentBeginOverlap.AddDynamic(this, &ASelectionBox::OnBeginOverlap);
    collider->OnComponentEndOverlap.AddDynamic(this, &ASelectionBox::OnEndOverlap);
}
void ASelectionBox::SetVisibility(const bool _isVisible, const bool _isRotating)
{
    isVisible = _isVisible, isRotating = _isRotating;
    
    if (isVisible || _isRotating)
        ScaleArea(0, 0);
    mesh->SetVisibility(_isVisible);
}
void ASelectionBox::ScaleArea(float width, float height) const
{
    width  /= 100;
    height /= 100;
    constexpr float expansion = 0;
    
    mesh->SetWorldScale3D(FVector(0.001f, width, height));
    collider->SetWorldScale3D(FVector(1, width + expansion, height + expansion));
}

// ReSharper disable CppMemberFunctionMayBeConst
void ASelectionBox::OnBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp
                                   , int otherBodyIndex, bool fromSweep, const FHitResult& sweepResult)
{
    if (!isRotating)
        Select(otherActor, true);
}
void ASelectionBox::OnEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp
    , int otherBodyIndex)
{
    const auto& controller = GetWorld()->GetFirstPlayerController();

    if (!controller->IsInputKeyDown(EKeys::LeftShift) && !isRotating)
    if (otherActor != ACreatorTile::FirstSelectedTile())
        Select(otherActor, false);
}
// ReSharper restore CppMemberFunctionMayBeConst

void ASelectionBox::Select(AActor* otherActor, const bool isSelected) const
{
    ACreatorTile* creatorTile = Cast<ACreatorTile>(otherActor);

    if (!creatorTile)
        return;
    creatorTile->Select(isSelected, true);
}

