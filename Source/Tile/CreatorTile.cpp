#include "CreatorTile.h"
#include "Logger.h"

ACreatorTile::ACreatorTile(): ATile() { }

void ACreatorTile::Tick(const float deltaSeconds)
{
    Super::Tick(deltaSeconds);

    if (!isPlayingSelectAnim)
        return;
    selectAlpha += deltaSeconds / 0.125f;
    
    const FVector newScale = FMath::Lerp(startScale, targetScale, selectAlpha);
    SetActorScale3D(newScale);

    if (selectAlpha > 1)
        isPlayingSelectAnim = false;
}
void ACreatorTile::NotifyActorOnClicked(FKey ButtonPressed)
{
    isSelected  = !isSelected;
    targetScale = isSelected ? FVector(0.8f) : FVector(1);
    startScale  = GetActorScale();
    selectAlpha = 0;
    isPlayingSelectAnim = true;
}
