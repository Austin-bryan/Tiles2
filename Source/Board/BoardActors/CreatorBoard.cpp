#include "CreatorBoard.h"

#include "AntMarchLineBase.h"
#include "CreatorRotator.h"
#include "CreatorTile.h"
#include "DeselectBoard.h"
#include "DragSelect.h"
#include "SeedCreator.h"
#include "ShortcutDetector.h"
#include "Kismet/GameplayStatics.h"

ACreatorBoard::ACreatorBoard()
{
    dragSelect = CreateDefaultSubobject<UDragSelect>(TEXT("Drag Select"));
    dragSelect->RegisterComponent();
    dragSelect->PrimaryComponentTick.bCanEverTick = true;
    dragSelect->SetComponentTickEnabled(true);
    dragSelect->PrimaryComponentTick.SetTickFunctionEnable(true);
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

    clipboard = Clipboard(this);
}
void ACreatorBoard::BeginPlay()
{
    Super::BeginPlay();
    const FVector location = GetActorLocation() - FVector(0, 100, 0);

    SeedCreator = Cast<ASeedCreator>(GetWorld()->SpawnActor(ASeedCreator::StaticClass()));
    SeedCreator->SetBoard(this);
    clipboard->BindToSeedChange(SeedCreator);

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
void ACreatorBoard::Tick(float DeltaSeconds) { clipboard->Tick(); }
//119, 50, 17
