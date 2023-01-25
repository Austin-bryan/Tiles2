#pragma once
#include "CoreMinimal.h"
#include "CreatorBoard.h"
#include "SelectionDrawer.h"
#include "DragSelect.generated.h"

class ASelectionBox;
class ACreatorBoard;
class ULineBatchComponent;
struct FBatchedLine;
enum class ESelectionShape : uint8;

/**
 * Handles selection of creator tiles via dragging.
 * Delegates drawing to SelectionDrawer, which determines if what shape the selection box is in.
 */

UCLASS()
class TILES2_API UDragSelect : public UActorComponent
{
    GENERATED_BODY()
public:
    UDragSelect();

    void OnRotate() const;
    void BeginPlay() override;
    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* ThisTickFunction) override;
    ESelectionShape Shape() const { return shape; }

    UFUNCTION(BlueprintCallable, Category="Default")
        void ChangeSelectionShape(ESelectionShape _shape);
protected:
    UPROPERTY()
        ULineBatchComponent* lineBatchComponent;
    UPROPERTY()
        ASelectionBox* selectionBox;
    TOptional<FVector> anchorPoint;
private:
    ACreatorBoard* GetBoard() const { return Cast<ACreatorBoard>(GetOwner()); }
    FRotator rotation;
    TUniquePtr<SelectionDrawer> drawer;
    ESelectionShape shape;
};
