#pragma once
#include "CoreMinimal.h"
#include "TileSide.generated.h"

class AModTile;
class ATileModule;
enum class ETileColor : uint8;
enum class EModule : uint8;
using FModuleMap = TMap<EModule, ATileModule*>;

UCLASS()
class ATileSide : public AActor
{
    GENERATED_BODY()
public:
    ETileColor Color() const;
    AModTile* ModTile() const;
    ATileModule* GetModule(EModule module);
    bool HasModule(EModule module) const;
    TArray<ATileModule*> Modules() const;

    ATileSide();

    void SetColor(ETileColor _color);
    void SetModTile(AModTile* _modTile);
    void BeginPlay() override;
    
    // ---- Module Manipulation ---- /
    void AddModule(ATileModule* module);
    void RemoveModule(EModule module);
    void RemoveAll();

    UPROPERTY(VisibleAnywhere)
    int XX = 10;

    UPROPERTY(VisibleAnywhere)
    USceneComponent* Root;
private:
    FModuleMap moduleMap;
    ETileColor color;
    
    UPROPERTY()
    AModTile* modTile;
};