#include "TileModules/BandagedModule.h"

#include "Board.h"
#include "ParameterParseState.h"
#include "ForwardDeclares.h"
#include "SqrCoord.h"

void UBandagedModule::ApplyParameters(const TArray<FParameter>& parameters)
{
	const Tiles& tiles = ModTile->Board().GetTiles();
	const FCoordPtr min = parameters[0].Get<FCoordPtr>();
	const FCoordPtr max = parameters[parameters.Num() - 1].Get<FCoordPtr>();
	
	FCoordPtr coord = parameters[0].Get<FCoordPtr>();

	TMap<FSqrCoord, int> map;
	map.Add(FSqrCoord(1, 1), 11);
	map.Add(FSqrCoord(1, 2), 12);
	map.Add(FSqrCoord(1, 3), 13);
	Log(map.Contains(FSqrCoord(1, 2)), FColor::Red);

	TMap<FCoordPtr, int, std::owner_less<FCoordPtr>> mapPtr;
	mapPtr.Add(MakeShared<FSqrCoord>(1, 1), 11);
	mapPtr.Add(MakeShared<FSqrCoord>(1, 2), 12);
	mapPtr.Add(MakeShared<FSqrCoord>(1, 3), 13);
	Log(mapPtr.Contains(MakeShared<FSqrCoord>(1, 2)));

	// const auto c1 = MakeShared<FSqrCoord>(0, 0);
	// const auto c2 = MakeShared<FSqrCoord>(0, 0);

	// Log("count coord: "_f + fstr(tiles.count(coord) == 1));
	// Log("count min: "_f   + fstr(tiles.count(min) == 1));
	// Log("count max: "_f   + fstr(tiles.count(max) == 1));
	// Log("equality: "_f    + fstr(c1 == c2));
	// Log("FCoord: "_f      + fstr(tiles.find(c1) != ModTile->Board().GetTiles().end()));
	// Log("FCoord: "_f      + fstr(tiles.count(c1) == 1));

	// do
	// {
	// 	// coord += EDirection::Right;
	// 	// Log("CUrrent coord: "_f + coord->ToString());
	// 	//
	// 	// if (!tiles.count(coord))
	// 	// {
	// 	// 	Log("Out of Bounds: "_f + coord->ToString());
	// 	// 	break;
	// 	// }
	// 	//
	// 	// Log(ModTile->Board().GetTiles().count(ModTile->GetCoord()));
	// 	
	// 	if (ModTile)
	// 	{
	// 		if (ModTile->Board().At(coord))
	// 		{
	// 			ModTile->Board().At(coord)->Destroy();
	// 			Log("Coord to destroy:"_f + coord->ToString());
	// 		} else Log("Null tile");
	// 	} else Log("Null ModTIle");
	// } while (coord->X() <= max->X());
	//
	// for(const auto& coordTilePair : ModTile->Board().GetTiles())
	// {
	// 	Log(coordTilePair.Key->ToString());
	// 	if (coordTilePair.Value->GetCoord() != ModTile->GetCoord())
	// 	{
	// 		// coordTilePair.Value->Destroy();
	// 		// ModTile->Board().At(coordTilePair.Key) = ModTile;
	// 	}
	// }
}

void UBandagedModule::BeginPlay() { Super::BeginPlay(); }
void UBandagedModule::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}