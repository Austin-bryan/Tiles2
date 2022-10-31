#pragma once
#include "CoreMinimal.h"
#include "DragSelect.generated.h"

UCLASS()
class UDragSelect : public UObject
{
    GENERATED_BODY()
public:
    UDragSelect() { }
    explicit UDragSelect(const int lineCount);
    virtual void Select(){}
    virtual void Draw(){}
protected:
    int lineCount = 4;
};
