#pragma once
#include "CoreMinimal.h"
#include "CreatorRotator.generated.h"

class ACreatorBoard;

UCLASS()
class UCreatorRotator : public UActorComponent
{
    GENERATED_BODY()
public:
    UCreatorRotator();
    void TickComponent(float deltaTime, ELevelTick tickType
                       , FActorComponentTickFunction* thisTickFunction) override;
    void SetBoard(ACreatorBoard* _board) { board = _board; }
protected:
    UPROPERTY()
        ACreatorBoard* board;
    UPROPERTY()
        UInputComponent* InputComponent;
private:
    void ResetRotation();
    float newRoll = 0;
    float increment = 15;

    bool rDown, lDown;
};