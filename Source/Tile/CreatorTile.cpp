#include "CreatorTile.h"
#include "Kismet/GameplayStatics.h"

TArray<ACreatorTile*> ACreatorTile::SelectedTiles;
ACreatorTile::ACreatorTile() : ATile() { }

void ACreatorTile::Tick(const float deltaSeconds)
{
    Super::Tick(deltaSeconds);

    if (activeAnimation)
        activeAnimation->Tick(deltaSeconds);
}
void ACreatorTile::EmptySelectedTiles() { SelectedTiles.Empty(); }
void ACreatorTile::NotifyActorOnClicked(FKey ButtonPressed) { Select(!isSelected); }

// todo:: this bool should be whether or not we're dragging
// todo: allow shift select when individually clicking but not dragging
// todo:: allow shift select and override select for dragging
void ACreatorTile::Select(const bool _isSelected, const bool isDragSelecting)
{
    if (_isSelected && isSelected || !_isSelected && !isSelected)
        return;
    isSelected = _isSelected;
    if (isSelected &&  !isDragSelecting)
    {
        if (const auto& controller = GetWorld()->GetFirstPlayerController();
            !controller->IsInputKeyDown(EKeys::LeftShift))
        {
            const auto selected = SelectedTiles;
            for (const auto& tile : selected)
                static_cast<ACreatorTile*>(tile)->Select(false);
        }
    }
    animPress.Play(isSelected);
    activeAnimation = &animPress;
    
    if (isSelected)
         SelectedTiles.Add(this);
    else SelectedTiles.Remove(this);
}