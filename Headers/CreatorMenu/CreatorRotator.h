#pragma once
#include "CoreMinimal.h"
#include "CreatorRotator.generated.h"

class UDragSelect;
class ACreatorBoard;
enum class ESelectionType : uint8;

UCLASS()
class TILES2_API UCreatorRotator : public UActorComponent
{
    GENERATED_BODY()
public:
    void SetBoard(ACreatorBoard* _board) { board = _board; }
    void SetDragSelect(UDragSelect* _dragSelect) { dragSelect = _dragSelect; }

    UFUNCTION(BlueprintCallable, Category="Default")
    void SetSelectionType(ESelectionType _selectionType);
protected:
    ACreatorBoard* board = nullptr;
private:
    UDragSelect* dragSelect;
    ESelectionType selectionType;
    float newRoll = 0;
    float increment = 15;
    bool rDown, lDown;
};