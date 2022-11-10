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
    UPROPERTY(EditInstanceOnly)
        float scale = 56.0f;
    UPROPERTY()
        ASelectionBox* selectionBox;
    UPROPERTY(EditInstanceOnly)
        float thickness = 4;
    UPROPERTY(EditInstanceOnly)
        float angleMultiplier = 57.25f;
    UPROPERTY(EditInstanceOnly)
        int vertexCount = 128;
    TOptional<FVector> anchorPoint;
private:
    FRotator rotation;
    bool isDragging;

    TUniquePtr<SelectionDrawer> drawer;
};
