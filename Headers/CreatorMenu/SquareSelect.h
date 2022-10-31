#pragma once
#include "CoreMinimal.h"
#include "DragSelect.h"
#include "SquareSelect.generated.h"

UCLASS()
class USquareSelect : public UDragSelect
{
    GENERATED_BODY()
public:
    USquareSelect() { }

    explicit USquareSelect(const int lineCount) : UDragSelect(lineCount) { }
    void Select() override;
    void Draw() override;
};
