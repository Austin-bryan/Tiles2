#include "ModuleFactory.h"

#include "Board.h"
#include "Enums.h"
#include "ModTile.h"
#include "TileModule.h"
#include "ParameterParseState.h"

class ABandagedModule;
class ACamoModule;

ATileModule* ModuleFactory::Produce(const EModule module, AModTile* modTile, const TArray<FParameter>& parameters) 
{
    switch(module)
    {
    // case EModule::Normal:         return nullptr;
    // case EModule::Gap:            return UTileModule::Create<AGapModuleModule>(modTile, parameters);
    // case EModule::Wrap:           return UTileModule::Create<AWrapModule>(modTile, parameters);
    case EModule::Camo:              return ATileModule::Create<ACamoModule>(modTile, parameters);
    // case EModule::TeleportWrap:   return UTileModule::Create<ATeleportWrapModule>(modTile, parameters);
    // case EModule::MirrorWrap:     return UTileModule::Create<AMirrorWrapModule>(modTile, parameters);
    // case EModule::CircuitWrap:    return UTileModule::Create<ACircuitWrapModule>(modTile, parameters);
    // case EModule::Link:           return UTileModule::Create<ALinkModule>(modTile, parameters);
    // case EModule::Rift:           return UTileModule::Create<ARiftModule>(modTile, parameters);
    // case EModule::Iron:           return UTileModule::Create<AIronModule>(modTile, parameters);
    // case EModule::Cloud:          return UTileModule::Create<ACloudModule>(modTile, parameters);
    // case EModule::Swap:           return UTileModule::Create<ASwapModule>(modTile, parameters);
    // case EModule::WrapWall:       return UTileModule::Create<AWrapWallModule>(modTile, parameters);
    // case EModule::NoSpawn:        return UTileModule::Create<ANoSpawnModule>(modTile, parameters);
    // case EModule::Rotator:        return UTileModule::Create<ARotatorModule>(modTile, parameters);
    // case EModule::CorrectCounter: return UTileModule::Create<ACorrectCounterModule>(modTile, parameters);
    case EModule::Bandaged:          return ATileModule::Create<ABandagedModule>(modTile, parameters);
    default: return nullptr;
    }
}
