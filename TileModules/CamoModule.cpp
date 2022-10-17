#include "TileModules/CamoModule.h"

UCamoModule::~UCamoModule() {}

void UCamoModule::BeginPlay()
{
	Super::BeginPlay();
	const auto mesh = Cast<UStaticMeshComponent>(ModTile->FindComponentByClass<UStaticMeshComponent>());
	const auto mat = mesh->GetMaterial(0);
	const auto dynamicMat = UMaterialInstanceDynamic::Create(mat, ModTile);

	dynamicMat->SetScalarParameterValue("ShowCamo", 1);
	mesh->SetMaterial(0, dynamicMat);
}
void UCamoModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}