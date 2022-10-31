#include "CreatorTile.h"
#include "Kismet/GameplayStatics.h"

TArray<ACreatorTile*> ACreatorTile::SelectedTiles;
ACreatorTile::ACreatorTile(): ATile() { }

void ACreatorTile::Tick(const float deltaSeconds)
{
    Super::Tick(deltaSeconds);

    if (activeAnimation)
        activeAnimation->Tick(deltaSeconds);
}
void ACreatorTile::EmptySelectedTiles()
{
    SelectedTiles.Empty();
}
void ACreatorTile::NotifyActorOnClicked(FKey ButtonPressed)
{
    if (isSelected)
         Deselect();
    else Select();
}
// todo:: this bool should be whether or not we're dragging
// todo: allow shift select when individually clicking but not dragging
// todo:: allow shift select and override select for dragging
void ACreatorTile::Select(const bool isDragSelecting)
{
    if (isSelected)
        return;

    if (!isDragSelecting)
    {
        if (const auto& controller = GetWorld()->GetFirstPlayerController();
            !controller->IsInputKeyDown(EKeys::LeftShift))
        {
            const auto selected = SelectedTiles;
            for (const auto& tile : selected)
                static_cast<ACreatorTile*>(tile)->Deselect();
        }
    }
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


