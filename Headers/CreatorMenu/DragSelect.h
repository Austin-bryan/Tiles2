#pragma once
#include "CoreMinimal.h"
#include "DragSelect.generated.h"

class ACreatorBoard;
class ULineBatchComponent;

UCLASS()
class UDragSelect : public UActorComponent
{
    GENERATED_BODY()
public:
    UDragSelect();

    void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    void Select();
    void Draw(FVector&& worldPosition, bool shouldDeselect);
    void SetBoard(ACreatorBoard* _board);

protected:
    UPROPERTY()
    ACreatorBoard* board;

    UPROPERTY()
    ULineBatchComponent* lineBatchComponent;

    UPROPERTY(EditInstanceOnly)
    float scale = 57;

    UPROPERTY(EditInstanceOnly)
    float thickness = 5;
private:
    TOptional<FVector> firstClick;
};
