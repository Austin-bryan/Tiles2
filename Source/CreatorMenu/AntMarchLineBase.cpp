#include "AntMarchLineBase.h"
#include "AssetDir.h"
#include "PaperSprite.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"

AAntMarchLineBase::AAntMarchLineBase()
{
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
    StaticMesh->SetupAttachment(Root);
    StaticMesh->SetStaticMesh(LoadObjectFromPath<UStaticMesh>("/Game/Meshes/SM_AntMesh.SM_AntMesh"_f));

    auto dir = "/Game/Materials/Mat_PanningAnts_Inst.Mat_PanningAnts_Inst"_f;
    DynamicMaterial = UKismetMaterialLibrary::CreateDynamicMaterialInstance(
        GetWorld(),
        Cast<UMaterialInstance>(StaticLoadObject(UMaterialInstance::StaticClass(), nullptr, *dir)));
    StaticMesh->SetMaterial(0, DynamicMaterial);
}
void AAntMarchLineBase::SetEndPosition(FVector endPosition)
{
    StaticMesh->SetVisibility(true);
    auto distance = FVector::Distance(GetActorLocation(), endPosition);
    SetActorScale3D(FVector(distance, 1, 1));
    SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), endPosition));
    
    auto loc = GetActorLocation();
    SetActorLocation(FVector(loc.X, -270, loc.Z));

    DynamicMaterial->SetScalarParameterValue(FName("Speed"), 100.0f / distance);
    DynamicMaterial->SetScalarParameterValue(FName("Scale"), 50.0f / distance);
}
void AAntMarchLineBase::Stop() const { StaticMesh->SetVisibility(false); }