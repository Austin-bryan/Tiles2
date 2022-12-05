#pragma once
#include "CoreMinimal.h"
#include "ModuleFactory.h"
#include "TileColor.h"
#include "TileSideHandler.generated.h"

class AModTile;
class ATileSide;
class UTileModule;
enum class ETileColor : uint8;

UCLASS()
class UTileSideHandler : public USceneComponent
{
    GENERATED_BODY()
public:
    UTileSideHandler();

    int SideCount() const;
    int TileID() const;
    bool IsMultiSided() const;
    ATileSide* CurrentSide() const;
    TArray<UTileModule*> CurrentModules() const;
    
    ETileColor Color() const;
    int CurrentIndex() const;
    AModTile* ModTile() const;
    
    void SetColor(ETileColor _color) const;
    void SetModTile(AModTile* _modTile);

    // ---------------- Methods ---------------- //
    void BeginPlay() override;
    void InitModules() const;
    TArray<ATileSide*> GetSides() const;
    ATileSide* operator[](int index) const;

    // ---- Manipulating Sides ---- //
    void RemoveAll();
    void RemoveSide(int index = -1);
    void AddSide(ETileColor color) const;
    bool HasModule(EModule module) const;
    UTileModule* GetModule(EModule module) const;

    // ---- Changing Sides ---- //
    void SetToDefault();
    void NextSide();
    void PrevSide();
    void ChangeSide(int newSide, bool shouldHide = true);

private:
    int currentIndex;

    UPROPERTY()
    AModTile* modTile;
};