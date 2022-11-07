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

    // TODO:: this class is used to deselect any tile when clicked off screen
    // TODO:: encaplesate selected tiles behind const getter and use dependencey injection to give it to DeselectBoard. Perhaps CreatorBoard should hold onto selected tiles, and spawn in all actors that need to modify it, while publicly its const getter for those who just need to read it
    const auto selected = ACreatorTile::SelectedTiles;
    for (const auto& tile : selected)
        tile->Select(false);
}