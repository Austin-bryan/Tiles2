#include "CreatorTile.h"
#include "Logger.h"

TArray<ACreatorTile*> ACreatorTile::SelectedTiles;

ACreatorTile::ACreatorTile(): ATile()
{
    
}

void ACreatorTile::Tick(const float deltaSeconds)
{
    Super::Tick(deltaSeconds);

    if (activeAnimation)
        activeAnimation->Tick(deltaSeconds);
}
void ACreatorTile::NotifyActorOnClicked(FKey ButtonPressed)
{
    animPress.Toggle();
    isSelected = !isSelected;
    activeAnimation = &animPress;

    if (isSelected)
         SelectedTiles.Add(this);
    else SelectedTiles.Remove(this);
}

void ACreatorTile::NotifyActorBeginCursorOver()
{
    if (isSelected)
        return;
    animHover.PlayForwards();
    activeAnimation = &animHover;
}
void ACreatorTile::NotifyActorEndCursorOver()
{
    if (isSelected)
        return;
    animHover.PlayReverse();
    activeAnimation = &animHover;
}
