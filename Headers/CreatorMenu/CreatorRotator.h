#pragma once
#include "CoreMinimal.h"
#include "Logger.h"
#include "CreatorRotator.generated.h"

class ACreatorBoard;
enum class ESelectionType : uint8;

UCLASS()
class UCreatorRotator : public UActorComponent
{
    GENERATED_BODY()
public:
    UCreatorRotator();
    void TickComponent(float deltaTime, ELevelTick tickType
                       , FActorComponentTickFunction* thisTickFunction) override;
    void SetBoard(ACreatorBoard* _board) { board = _board; }

    UFUNCTION(BlueprintCallable, Category="Default")
    void SetSelectionType(ESelectionType _selectionType);
protected:
    ACreatorBoard* board = nullptr;
private:
    ESelectionType selectionType;
    void ResetRotation() const;
    float newRoll = 0;
    float increment = 15;
    bool rDown, lDown;
};