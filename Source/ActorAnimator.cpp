#include "ActorAnimator.h"

ActorAnimator::ActorAnimator(USceneComponent* target, FVector start, FVector end, const EAnimMode animMode, const float speed) :
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

void ActorAnimator::FinishAnimation()   { ApplyAnimation(animState == EAnimState::Forwards ? end : begin); }
void ActorAnimator::Play(bool forwards) { SetupAnim(forwards ? 0 : 1
    , forwards ? EAnimState::Forwards : EAnimState::Backwards, forwards); }

void ActorAnimator::SetupAnim(const float _alpha, const EAnimState _animState, const bool _isSelected)
{
    alpha = _alpha, animState = _animState, isSelected = _isSelected;
}
void ActorAnimator::Toggle() { Play(!isSelected); }
void ActorAnimator::ApplyAnimation(const FVector value)
{
    switch (animMode)
    {
    case EAnimMode::Scale:    target->SetWorldScale3D(value);  break;
    case EAnimMode::Position: target->SetWorldLocation(value); break;
    default: throw std::invalid_argument("Invalid animation mode.");
    }
}