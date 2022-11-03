#pragma once
#include "CoreMinimal.h"
#include "DragSelect.generated.h"

class ASelectionBox;
class ACreatorBoard;
class ULineBatchComponent;

struct FBatchedLine;

UCLASS()
class UDragSelect : public UActorComponent
{
    GENERATED_BODY()
public:
    UDragSelect();

    void BeginPlay() override;

    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* ThisTickFunction) override;
    void Select();
    void SetBoard(ACreatorBoard* _board);
    
    void Draw(TArray<FBatchedLine>& lines, FVector&& worldPosition);
    void Select(const TArray<FBatchedLine>& lines, bool shouldDeselect);
protected:
    UPROPERTY()
    ACreatorBoard* board;

    UPROPERTY()
    ULineBatchComponent* lineBatchComponent;

    UPROPERTY(EditInstanceOnly)
    float scale = 57;
    
    UPROPERTY()
    ASelectionBox* selectionBox;    

    UPROPERTY(EditInstanceOnly)
    float thickness = 4;
private:
    TOptional<FVector> firstClick;
};
