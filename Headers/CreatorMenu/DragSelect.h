#pragma once
#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
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
    void Draw(FVector&& worldPosition, FVector& vert1);
protected:
    UPROPERTY()
        ACreatorBoard* board;
    UPROPERTY()
        ULineBatchComponent* lineBatchComponent;
    UPROPERTY(EditInstanceOnly)
        float scale = 56.0f;
    UPROPERTY()
        ASelectionBox* selectionBox;
    UPROPERTY(EditInstanceOnly)
        float thickness = 4;
    
    TOptional<FVector> firstClick;
    TOptional<FVector> secondClick;
private:
    FRotator rotation;
};
