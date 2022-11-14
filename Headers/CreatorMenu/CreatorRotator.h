#pragma once
#include "CoreMinimal.h"
#include "CreatorRotator.generated.h"

class UDragSelect;
class ACreatorBoard;
enum class ESelectionAngle : uint8;

/**
 * For the different angles of box selection to work, this class rotates the board and camera accordingly
 * to create the illusion the selection box is angled differently.
 */
UCLASS()
class TILES2_API UCreatorRotator : public UActorComponent
{
    GENERATED_BODY()
public:
    void SetBoard(ACreatorBoard* _board) { board = _board; }
    void SetDragSelect(UDragSelect* _dragSelect) { dragSelect = _dragSelect; }

    UFUNCTION(BlueprintCallable, Category="Default")
    void SetSelectionType(ESelectionAngle _selectionType);
protected:
    ACreatorBoard* board = nullptr;
private:
    UDragSelect* dragSelect;
    ESelectionAngle selectionType;
    float newRoll = 0;
    float increment = 15;
    bool rDown, lDown;
};