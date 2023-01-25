#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileSide.generated.h"

class AModTile;
class ATileModule;
class UMaterialInstanceDynamic;
enum class ETileColor : uint8;
enum class EModule : uint8;
using FModuleMap = TMap<EModule, ATileModule*>;

UCLASS()
class ATileSide : public AActor
{
    GENERATED_BODY()
public:
    ETileColor Color() const;
    void SetColor(ETileColor _tileColor, bool propagate);

    AModTile* ModTile() const;
    ATileModule* GetModule(EModule module);
    bool HasModule(EModule module) const;
    TArray<ATileModule*> Modules() const;

    ATileSide();
    void Tick(float DeltaSeconds) override;
    void SetModTile(AModTile* _modTile);
    void BeginPlay() override;
    
    // ---- Module Manipulation ---- /
    void AddModule(ATileModule* module);
    void RemoveModule(const ATileModule* module);
    void RemoveModule(EModule module, bool shouldDestroy = true);
    void RemoveAll();

    UPROPERTY(VisibleAnywhere)
        int XX = 10;
    UPROPERTY(VisibleAnywhere)
        USceneComponent* Root;
private:
    FModuleMap moduleMap;
    ETileColor tileColor;
    
    UPROPERTY() AModTile* modTile;
    UPROPERTY() UMaterialInstanceDynamic* instance;
};