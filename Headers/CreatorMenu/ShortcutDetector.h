#pragma once
#include "CoreMinimal.h"
#include "Logger.h"
#include "ShortcutDetector.generated.h"

class UDragSelect;
class UCreatorRotator;
class ACreatorBoard;
using ModifiedShortcuts = TMap<FKey, TFunction<void()>>;

enum EKey;

UCLASS()
class UShortcutDetector : public UActorComponent
{
    GENERATED_BODY()
public:
    UShortcutDetector();
    void BeginPlay() override;
    UShortcutDetector* SetBoard(ACreatorBoard* _creatorBoard) { creatorBoard = _creatorBoard; return this; }
    UShortcutDetector* SetRotator(UCreatorRotator* _rotator) { rotator = _rotator; return this; }
    UShortcutDetector* SetDragSelect(UDragSelect* _dragSelect) { dragSelect = _dragSelect; return this; }

    UFUNCTION(BlueprintCallable)
    void CallShortcut(const FKey key) const
    {
        const int modifier = Alt() << 2 | Ctrl() << 1 | Shift() << 0;

        if (shortcuts[modifier]->Contains(key))
            (*shortcuts[modifier])[key]();
    }
private:
    APlayerController* controller;
    ACreatorBoard* creatorBoard; 
    UCreatorRotator* rotator;
    UDragSelect* dragSelect;
    
    TArray<ModifiedShortcuts*> shortcuts;
    void AnyKey(FKey key);

    bool Key(FKey key) const;
    bool KeyDown(FKey key) const;
    bool Shift() const;
    bool Ctrl() const;
    bool Alt() const;
};