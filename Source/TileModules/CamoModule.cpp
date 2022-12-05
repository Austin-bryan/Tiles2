#include "CamoModule.h"
#include "ModTile.h"
#include "ProceduralMeshComponent.h"

UCamoModule::~UCamoModule() {}

void UCamoModule::BeginPlay()
{
	// TODO:: Create helper method to get components
	Super::BeginPlay();
	const auto mesh = Cast<UProceduralMeshComponent>(ModTile->GetComponentByClass(UProceduralMeshComponent::StaticClass()));
	const auto mat = Cast<UMaterialInstanceDynamic>(
		Cast<UProceduralMeshComponent>(ModTile->GetComponentByClass(UProceduralMeshComponent::StaticClass()))->GetMaterial(0));
	mat->SetScalarParameterValue("ShowCamo", 1);
	mesh->SetMaterial(0, mat);
}