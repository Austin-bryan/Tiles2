#include "TileModules/BandagedModule.h"
#include "Board.h"
#include "ParameterParseState.h"
#include "ForwardDeclares.h"
#include "HexCoord.h"
#include "SqrCoord.h"
#include "TilesMap.h"

/*
 * todo:: Create facade class for TMap<FCoordPtr, ATile*>
 * todo: allow retriving of FCoordPtr by value, not by ref
 * todo: forward basic functionality to underlying type
 * todo: replace Tiles in the include to TilesMap
 * todo: have methods for returning keys or values like in c#
 */
void UBandagedModule::ApplyParameters(const TArray<FParameter>& parameters)
{
	const Tiles& tiles  = ModTile->Board().GetTiles();
	const FCoordPtr min = parameters[0].Get<FCoordPtr>();
	const FCoordPtr max = parameters[parameters.Num() - 1].Get<FCoordPtr>();
	FCoordPtr coord     = min;

	do
	{
		coord += EDirection::Right;
		if (!ModTile->Board().GetTiles().Contains(coord))
			return;
		if (ModTile)
		{
			if (ModTile->Board().At(coord))
			{
				ModTile->Board().At(coord)->Destroy();
			}
		}
	}
	while (coord->X() <= max->X());
}

void UBandagedModule::BeginPlay() { Super::BeginPlay(); }
void UBandagedModule::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}