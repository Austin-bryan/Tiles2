#pragma once
#include "CoreMinimal.h"
#include "ModuleFactory.h"
#include "TileColor.h"
#include "Components/SceneComponent.h"
#include "TileSideHandler.generated.h"

class AModTile;
class ATileSide;
class ATileModule;
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
    TArray<ATileModule*> CurrentModules() const;
    
    ETileColor Color() const;
    int CurrentIndex() const;
    AModTile* ModTile() const;

    // ---------------- Methods ---------------- //
    void SetModTile(AModTile* _modTile);
    void BeginPlay() override;
    void InitModules() const;
    TArray<ATileSide*> GetSides() const;
    ATileSide* operator[](int index) const;

    // ---- Transform ---- //
    void PropagateSideScale(FVector scale) const;
    void PropagateSideLocation(FVector location) const;

    // ---- Manipulating Sides ---- //
    void RemoveAll();
    void RemoveSide(int index = -1);
    void AddSide(ETileColor color) const;
    bool HasModule(EModule module) const;
    ATileModule* GetModule(EModule module) const;

    // ---- Changing Sides ---- //
    void SetToDefault();
    void NextSide();
    void PrevSide();
    void ChangeSide(int newSide, bool shouldHide = true);

    UPROPERTY(VisibleAnywhere)
    FString name;
private:
    int currentIndex;

    UPROPERTY()
    AModTile* modTile;
};