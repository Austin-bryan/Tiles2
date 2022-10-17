#include "TileModules/ModuleFactory.h"
#include "Parsing/ParseStates/ParameterParseState.h"
#include "TileModule.h"
#include "Enums.h"

class UCamoModule;

UTileModule* ModuleFactory::Produce(const EModule module, ATile* modTile, const TArray<FParameter>* const parameters) 
{
    switch(module)
    {
    // case EModule::Normal:         return nullptr;
    // case EModule::Gap:            return UTileModule::Create<GapModuleModule>(modTile);
    // case EModule::Wrap:           return UTileModule::Create<WrapModule>(modTile);
    case EModule::Camo:           return UTileModule::Create<UCamoModule>(modTile);
    // case EModule::TeleportWrap:   return UTileModule::Create<TeleportWrapModule>(modTile);
    // case EModule::MirrorWrap:     return UTileModule::Create<MirrorWrapModule>(modTile);
    // case EModule::CircuitWrap:    return UTileModule::Create<CircuitWrapModule>(modTile);
    // case EModule::Link:           return UTileModule::Create<LinkModule>(modTile);
    // case EModule::Rift:           return UTileModule::Create<RiftModule>(modTile);
    // case EModule::Iron:           return UTileModule::Create<IronModule>(modTile);
    // case EModule::Cloud:          return UTileModule::Create<CloudModule>(modTile);
    // case EModule::Swap:           return UTileModule::Create<SwapModule>(modTile);
    // case EModule::WrapWall:       return UTileModule::Create<WrapWallModule>(modTile);
    // case EModule::NoSpawn:        return UTileModule::Create<NoSpawnModule>(modTile);
    //case EModule::Rotator: return UTileModule::Create<RotatorModule>(modTile);
    // case EModule::CorrectCounter: return UTileModule::Create<CorrectCounterModule>(modTile);
    //case EModule::Bandaged: return UTileModule::Create<BandagedModule>(modTile);
    default:                      return nullptr;
    }
}
