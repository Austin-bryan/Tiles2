#pragma once
#include "CoreMinimal.h"
#include "CreatorRotator.generated.h"

class ACreatorBoard;
enum class ESelectionType : uint8;

UCLASS()
class UCreatorRotator : public UActorComponent
{
    GENERATED_BODY()
public:
    void SetBoard(ACreatorBoard* _board) { board = _board; }

    UFUNCTION(BlueprintCallable, Category="Default")
    void SetSelectionType(ESelectionType _selectionType);
protected:
    ACreatorBoard* board = nullptr;
private:
    ESelectionType selectionType;
    float newRoll = 0;
    float increment = 15;
    bool rDown, lDown;
};