#pragma once
#include "CoreMinimal.h"
#include "SelectionDrawer.h"
#include "DragSelect.generated.h"

class ASelectionBox;
class ACreatorBoard;
class ULineBatchComponent;
struct FBatchedLine;
enum class ESelectionType : uint8;

UCLASS()
class TILES2_API UDragSelect : public UActorComponent
{
    GENERATED_BODY()
public:
    UDragSelect();

    void BeginPlay() override;
    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* ThisTickFunction) override;
    void SetBoard(ACreatorBoard* _board) { board = _board; }
    void OnRotate() const;
    void ChangeSelectionShape(ESelectionType mode);
protected:
    UPROPERTY()
        ACreatorBoard* board;
    UPROPERTY()
        ULineBatchComponent* lineBatchComponent;
    UPROPERTY()
        ASelectionBox* selectionBox;
    TOptional<FVector> anchorPoint;
private:
    FRotator rotation;
    TUniquePtr<SelectionDrawer> drawer;
};
