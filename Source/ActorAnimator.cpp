#include "ActorAnimator.h"

ActorAnimator::ActorAnimator(AActor* target, FVector start, FVector end, const EAnimMode animMode, const float speed) :
    target{ target }, animMode{ animMode}, speed{ speed },
    animState{ EAnimState::Idle }, begin{ start }, end{ end }
{
    
}

void ActorAnimator::Tick(const float deltaSeconds)
{
    if (animState == EAnimState::Idle)
        return;
    if (alpha > 1 || alpha < 0)    
        FinishAnimation();
    else
    {
        ApplyAnimation(FMath::Lerp(begin, end, alpha));
        alpha += deltaSeconds * speed * (animState == EAnimState::Forwards ? 1 : -1);
    }
}

void ActorAnimator::FinishAnimation() { ApplyAnimation(animState == EAnimState::Forwards ? end : begin); }
void ActorAnimator::PlayForwards()    { SetupAnim(0, EAnimState::Forwards, true); }
void ActorAnimator::PlayReverse()     { SetupAnim(1, EAnimState::Backwards, false); }

void ActorAnimator::SetupAnim(const float _alpha, const EAnimState _animState, const bool _isSelected)
{
    alpha = _alpha, animState = _animState, isSelected = _isSelected;
}
void ActorAnimator::Toggle()
{
    if (!isSelected)
         PlayForwards();
    else PlayReverse();
}
void ActorAnimator::ApplyAnimation(const FVector value)
{
    switch (animMode)
    {
    case EAnimMode::Scale:    target->SetActorScale3D(value);  break;
    case EAnimMode::Position: target->SetActorLocation(value); break;
    default: throw std::invalid_argument("Invalid animation mode.");
    }
}