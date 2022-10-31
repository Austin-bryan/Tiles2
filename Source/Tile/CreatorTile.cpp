#include "CreatorTile.h"

TArray<ACreatorTile*> ACreatorTile::SelectedTiles;
ACreatorTile::ACreatorTile(): ATile() { }

void ACreatorTile::Tick(const float deltaSeconds)
{
    Super::Tick(deltaSeconds);

    if (activeAnimation)
        activeAnimation->Tick(deltaSeconds);
}
void ACreatorTile::NotifyActorOnClicked(FKey ButtonPressed)
{
    if (isSelected)
         Deselect();
    else Select();
}
void ACreatorTile::Select()
{
    if (isSelected)
        return;
    isSelected = true;
    animPress.PlayForwards();
    activeAnimation = &animPress;
    SelectedTiles.Add(this);
}
void ACreatorTile::Deselect()
{
    if (!isSelected)
        return;
    isSelected = false;
    animPress.PlayReverse();
    activeAnimation = &animPress;
    SelectedTiles.Remove(this);
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


