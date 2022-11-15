#include "CreatorBoard.h"
#include "CreatorRotator.h"
#include "CreatorTile.h"
#include "DeselectBoard.h"
#include "DragSelect.h"
#include "ShortcutDetector.h"
#include "Kismet/GameplayStatics.h"

ACreatorBoard::ACreatorBoard()
{
    dragSelect = CreateDefaultSubobject<UDragSelect>(TEXT("Drag Select"));
    dragSelect->RegisterComponent();
    dragSelect->PrimaryComponentTick.bCanEverTick = true;
    dragSelect->SetComponentTickEnabled(true);
    dragSelect->PrimaryComponentTick.SetTickFunctionEnable(true);
    dragSelect->SetBoard(this);
    AddInstanceComponent(dragSelect);

    rotator = CreateDefaultSubobject<UCreatorRotator>(TEXT("Creator Rotator"));
    rotator->RegisterComponent();
    rotator->PrimaryComponentTick.bCanEverTick = true;
    rotator->SetComponentTickEnabled(true);
    rotator->SetBoard(this);
    rotator->SetDragSelect(dragSelect);
    AddInstanceComponent(rotator);

    shortcutDetector = CreateDefaultSubobject<UShortcutDetector>(TEXT("Shortcut Detector"));
    shortcutDetector->RegisterComponent();
    shortcutDetector->SetBoard(this)->SetRotator(rotator)->SetDragSelect(dragSelect);
    AddOwnedComponent(shortcutDetector);
    AddInstanceComponent(shortcutDetector);
    
    rand = FMath::RandRange(0, 100000);
}
ACreatorBoard::~ACreatorBoard() { ACreatorTile::EmptySelectedTiles(); }

void ACreatorBoard::BeginPlay()
{
    Super::BeginPlay();
    const FVector location = GetActorLocation() - FVector(0, 100, 0);

    const auto deselectBoard = GetWorld()->SpawnActor(
        ADeselectBoard::StaticClass(),
        &location);
    deselectBoard->SetActorScale3D(FVector(200, 1, 200));
}

UDragSelect* ACreatorBoard::GetDragSelect() const { return dragSelect; }
UCreatorRotator* ACreatorBoard::GetCreatorRotator() const { return rotator; }

void ACreatorBoard::SetCreatorMenu(UCreatorMenu* _creatorMenu)
{
    creatorMenu = _creatorMenu;
    shortcutDetector->SetCreatorMenu(creatorMenu);
}

UClass* ACreatorBoard::TileClass() const { return ACreatorTile::StaticClass(); }
//119, 50, 17