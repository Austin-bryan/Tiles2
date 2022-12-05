#pragma once
#include "CoreMinimal.h"
#include "TileSide.generated.h"

class AModTile;
class UTileModule;
enum class ETileColor : uint8;
enum class EModule : uint8;
using FModuleMap = TMap<EModule, UTileModule*>;

UCLASS()
class UTileSide : public USceneComponent
{
    GENERATED_BODY()
public:
    ETileColor Color() const;
    AModTile* ModTile() const;
    UTileModule* GetModule(EModule module);
    bool HasModule(EModule module) const;
    TArray<UTileModule*> Modules() const;

    void SetColor(ETileColor _color);
    void SetModTile(AModTile* _modTile);
    void BeginPlay() override;
    
    // ---- Module Manipulation ---- /
    void AddModule(UTileModule* module);
    void RemoveModule(EModule module);
    void RemoveAll();

    UPROPERTY(VisibleAnywhere)
    int XX = 10;
private:
    FModuleMap moduleMap;
    ETileColor color;
    
    UPROPERTY()
    AModTile* modTile;
};