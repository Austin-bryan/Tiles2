#include "ModuleFactory.h"
#include "Enums.h"
#include "TileModule.h"
#include "ParameterParseState.h"
#include "RotatorModule.h"

class ABandagedModule;
class ACamoModule;

ATileModule* ModuleFactory::Produce(const EModule module, AModTile* modTile, const TArray<FParameter>& parameters) 
{
    switch(module)
    {
    // case EModule::Normal:         return nullptr;
    // case EModule::Gap:            return ATileModule::Create<AGapModuleModule>(modTile, parameters);
    // case EModule::Wrap:           return ATileModule::Create<AWrapModule>(modTile, parameters);
    case EModule::Camo:              return ATileModule::Create<ACamoModule>(modTile, parameters);
    // case EModule::TeleportWrap:   return ATileModule::Create<ATeleportWrapModule>(modTile, parameters);
    // case EModule::MirrorWrap:     return ATileModule::Create<AMirrorWrapModule>(modTile, parameters);
    // case EModule::CircuitWrap:    return ATileModule::Create<ACircuitWrapModule>(modTile, parameters);
    // case EModule::Link:           return ATileModule::Create<ALinkModule>(modTile, parameters);
    // case EModule::Rift:           return ATileModule::Create<ARiftModule>(modTile, parameters);
    // case EModule::Iron:           return ATileModule::Create<AIronModule>(modTile, parameters);
    // case EModule::Cloud:          return ATileModule::Create<ACloudModule>(modTile, parameters);
    // case EModule::Swap:           return ATileModule::Create<ASwapModule>(modTile, parameters);
    // case EModule::WrapWall:       return ATileModule::Create<AWrapWallModule>(modTile, parameters);
    // case EModule::NoSpawn:        return ATileModule::Create<ANoSpawnModule>(modTile, parameters);
    case EModule::Rotator:           return ATileModule::Create<ARotatorModule>(modTile, parameters);
    // case EModule::CorrectCounter: return ATileModule::Create<ACorrectCounterModule>(modTile, parameters);
    case EModule::Bandaged:          return ATileModule::Create<ABandagedModule>(modTile, parameters);
    default: return nullptr;
    }
}
