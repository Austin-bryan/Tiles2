#include "CreatorTile.h"

#include "TileSideHandler.h"
#include "Kismet/GameplayStatics.h"

TArray<ACreatorTile*> ACreatorTile::selectedTiles;
ACreatorTile* ACreatorTile::firstSelectedTile;

ACreatorTile::ACreatorTile() : AModTile()
{
    if (selectedTiles.Num() > 0)
        selectedTiles.Empty();
}

void ACreatorTile::Tick(const float deltaSeconds)
{
    Super::Tick(deltaSeconds);

    if (activeAnimation)
    {
        activeAnimation->Tick(deltaSeconds);
        SideHandler->PropagateSideScale(activeAnimation->Value());
    }
}
void ACreatorTile::NotifyActorOnClicked(FKey ButtonPressed)
{
    // Invert selection, unless was previously dragselected
    Select(!isSelected || wasDragSelected && isSelected);
}
void ACreatorTile::Select(const bool _isSelected, const bool isDragSelecting)
{
    isSelected = _isSelected;

    // Shift selection
    if (isSelected && !isDragSelecting)
    {
        firstSelectedTile = this;

        // Deselect other tiles except for self
        if (const auto& controller = GetWorld()->GetFirstPlayerController();
            !controller->IsInputKeyDown(EKeys::LeftShift))
        {
            const auto selected = selectedTiles;
            for (const auto& tile : selected)
                if (tile != this)
                    tile->Select(false);
        }
    }
    
    // Prevents animating into the state its already in
    if (isSelected && animPress.GetAnimState() != EAnimState::Forwards
    || !isSelected && animPress.GetAnimState() != EAnimState::Backwards)
    {
        animPress.Play(isSelected);
        activeAnimation = &animPress;
    }
    
    if (isSelected)
         selectedTiles.AddUnique(this);
    else selectedTiles.Remove(this);
    
    wasDragSelected = isDragSelecting;
}