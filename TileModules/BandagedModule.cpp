#include "TileModules/BandagedModule.h"
#include "Board.h"
#include "ParameterParseState.h"
#include "ForwardDeclares.h"
#include "HexCoord.h"
#include "TilesMap.h"

void UBandagedModule::ApplyParameters(const TArray<FParameter>& parameters)
{
	const Tiles& tiles  = ModTile->Board().GetTiles();
	const FCoordPtr min = parameters[0].Get<FCoordPtr>();
	const FCoordPtr max = parameters[parameters.Num() - 1].Get<FCoordPtr>();
	FCoordPtr coord     = min;

	do
	{
		coord += EDirection::Right;
		if (tiles.Contains(coord))
			tiles[coord]->Destroy();
	}
	while (coord->X() <= max->X());
}

void UBandagedModule::BeginPlay() { Super::BeginPlay(); }
void UBandagedModule::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}