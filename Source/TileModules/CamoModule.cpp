#include "CamoModule.h"
#include "BandagedModule.h"
#include "ModTile.h"
#include "ProceduralMeshComponent.h"

ACamoModule::~ACamoModule() { }
void ACamoModule::ApplyCamoTexture(const AModTile* tile) const
{
	const auto mesh = Cast<UProceduralMeshComponent>(tile->GetComponentByClass(UProceduralMeshComponent::StaticClass()));
	const auto mat = Cast<UMaterialInstanceDynamic>(
		Cast<UProceduralMeshComponent>(
			ModTile->GetComponentByClass(
				UProceduralMeshComponent::StaticClass()))->GetMaterial(0));
	mat->SetScalarParameterValue("ShowCamo", 1);
	mesh->SetMaterial(0, mat);
}
void ACamoModule::Init() const
{
	Super::Init();

	// Apply camo texture to all sibling tiles if needed
	if (auto b = Cast<ABandagedModule>(ModTile->CurrentSide()->GetModule(EModule::Bandaged)))
		for (auto tile : b->Tiles)
			ApplyCamoTexture(tile);
	else ApplyCamoTexture(ModTile);

}
void ACamoModule::BeginPlay() { Super::BeginPlay(); }