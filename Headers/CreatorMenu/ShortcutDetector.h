#pragma once
#include "CoreMinimal.h"
#include "ShortcutDetector.generated.h"

class ACreatorTile;
class UDragSelect;
class UCreatorRotator;
class ACreatorBoard;
class UCreatorMenu;
using ModifiedShortcuts = TMap<FKey, TFunction<void()>>;

enum EKey;

/**
 * Uses binary flags to determine which combination of modifiers is being used, to find the correct
 * function to execute.
 */

UCLASS()
class UShortcutDetector : public UActorComponent
{
    GENERATED_BODY()
public:
    UShortcutDetector();
    void BeginPlay() override;
    UShortcutDetector* SetBoard(ACreatorBoard* _creatorBoard)     { creatorBoard = _creatorBoard; return this; }
    UShortcutDetector* SetRotator(UCreatorRotator* _rotator)      { rotator      = _rotator;      return this; }
    UShortcutDetector* SetDragSelect(UDragSelect* _dragSelect)    { dragSelect   = _dragSelect;   return this; }
    UShortcutDetector* SetCreatorMenu(UCreatorMenu* _creatorMenu) { creatorMenu  = _creatorMenu;  return this; }

    UFUNCTION(BlueprintCallable)
    void CallShortcut(const FKey key) const;
private:
    enum class EModifier { None, Shift, Control, Alt };
    APlayerController* controller;
    ACreatorBoard* creatorBoard; 
    UCreatorRotator* rotator;
    UDragSelect* dragSelect;
    UCreatorMenu* creatorMenu;
    
    TArray<ModifiedShortcuts*> shortcuts;

    void SelectAdjacentTiles(
        ACreatorTile* creatorTile,
        TArray<ACreatorTile*>& creatorTiles);

    void AnyKey (FKey key);
    bool Key    (FKey key) const;
    bool KeyDown(FKey key) const;
    bool IsModifierDown(FKey left, FKey right, EModifier modifier) const;
    bool Shift() const;
    bool Ctrl() const;
    bool Alt() const;

    TMap<EModifier, float> modifierTimes;
};