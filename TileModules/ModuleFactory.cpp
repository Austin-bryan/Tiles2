#include "TileModules/ModuleFactory.h"
#include "Parsing/ParseStates/ParameterParseState.h"
#include "TileModule.h"
#include "Enums.h"

class UBandagedModule;
class UCamoModule;

UTileModule* ModuleFactory::Produce(const EModule module, ATile* modTile, const TArray<FParameter>& parameters) 
{
    switch(module)
    {
    // case EModule::Normal:         return nullptr;
    // case EModule::Gap:            return UTileModule::Create<UGapModuleModule>(modTile, parameters);
    // case EModule::Wrap:           return UTileModule::Create<UWrapModule>(modTile, parameters);
    case EModule::Camo:           return UTileModule::Create<UCamoModule>(modTile, parameters);
    // case EModule::TeleportWrap:   return UTileModule::Create<UTeleportWrapModule>(modTile, parameters);
    // case EModule::MirrorWrap:     return UTileModule::Create<UMirrorWrapModule>(modTile, parameters);
    // case EModule::CircuitWrap:    return UTileModule::Create<UCircuitWrapModule>(modTile, parameters);
    // case EModule::Link:           return UTileModule::Create<ULinkModule>(modTile, parameters);
    // case EModule::Rift:           return UTileModule::Create<URiftModule>(modTile, parameters);
    // case EModule::Iron:           return UTileModule::Create<UIronModule>(modTile, parameters);
    // case EModule::Cloud:          return UTileModule::Create<UCloudModule>(modTile, parameters);
    // case EModule::Swap:           return UTileModule::Create<USwapModule>(modTile, parameters);
    // case EModule::WrapWall:       return UTileModule::Create<UWrapWallModule>(modTile, parameters);
    // case EModule::NoSpawn:        return UTileModule::Create<UNoSpawnModule>(modTile, parameters);
    // case EModule::Rotator:        return UTileModule::Create<URotatorModule>(modTile, parameters);
    // case EModule::CorrectCounter: return UTileModule::Create<UCorrectCounterModule>(modTile, parameters);
    case EModule::Bandaged:       return UTileModule::Create<UBandagedModule>(modTile, parameters);
    default:                      return nullptr;
    }
}
