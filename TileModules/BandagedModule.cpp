#include "TileModules/BandagedModule.h"
#include "ParameterParseState.h"
#include "ForwardDeclares.h"

UBandagedModule::~UBandagedModule() {}

void UBandagedModule::ApplyParameters(const TArray<FParameter>& parameters)
{
	Log("Apply: ");
	for (const auto& parameter : parameters)
	{
		Log(parameter.ToString(), FColor::Purple);
		
		if (const float* number = parameter.GetIf<float>())
			ModTile->SetActorScale3D(FVector3d(*number, *number, *number));
		else if (const FCoordPtr* coord = parameter.GetIf<FCoordPtr>())
		{
			ModTile->SetCoord(*coord);
		}
	}
}

void UBandagedModule::BeginPlay() 
{
	Log("Begin");
	Super::BeginPlay();
	const auto mesh = Cast<UStaticMeshComponent>(ModTile->FindComponentByClass<UStaticMeshComponent>());
	const auto mat = mesh->GetMaterial(0);
	const auto dynamicMat = UMaterialInstanceDynamic::Create(mat, ModTile);

	dynamicMat->SetScalarParameterValue("ShowCamo", 1);
	mesh->SetMaterial(0, dynamicMat);
}
void UBandagedModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}