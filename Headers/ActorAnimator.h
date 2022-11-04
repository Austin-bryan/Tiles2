#pragma once
#include "CoreMinimal.h"
#include "Logger.h"

enum class EAnimMode  { Scale, Position };
enum class EAnimState { Idle, Forwards, Backwards };

class TILES2_API ActorAnimator
{
public:
    ActorAnimator(USceneComponent* target, FVector start, FVector end, EAnimMode animMode, float speed = 1);
    void Tick(float deltaSeconds);
    void Play(bool forwards);
    void Toggle();
    void FinishAnimation();
private:
    void SetupAnim(float _alpha, EAnimState _animState, bool _isSelected);
    void ApplyAnimation(FVector value);
    
    USceneComponent* target;
    bool isSelected = false;
    const EAnimMode animMode;
    const float speed;
    EAnimState animState;
    FVector begin, end;
    float alpha = 0;
};