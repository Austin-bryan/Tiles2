#pragma once
#include "DeselectBoard.h"
#include "CreatorTile.h"
#include "Components/BoxComponent.h"

ADeselectBoard::ADeselectBoard()
{
    Collider = CreateDefaultSubobject<UBoxComponent>(FName("Collider"));
    Collider->SetCollisionProfileName("BlockAll");
    Collider->RegisterComponent();
}

void ADeselectBoard::NotifyActorOnClicked(const FKey ButtonPressed)
{
    Super::NotifyActorOnClicked(ButtonPressed);

    const auto controller = GetWorld()->GetFirstPlayerController();
    if (controller->IsInputKeyDown(EKeys::LeftShift))
        return;
    
    const auto selected = ACreatorTile::SelectedTiles();
    for (const auto& tile : selected)
        tile->Select(false);
}