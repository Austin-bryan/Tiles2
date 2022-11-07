#include "CreatorBoard.h"
#include "CreatorRotator.h"
#include "CreatorTile.h"
#include "DeselectBoard.h"
#include "DragSelect.h"
#include "Kismet/GameplayStatics.h"

ACreatorBoard::ACreatorBoard()
{
    dragSelect = CreateDefaultSubobject<UDragSelect>(FName("Drag Select"));
    dragSelect->RegisterComponent();
    dragSelect->PrimaryComponentTick.bCanEverTick = true;
    dragSelect->SetComponentTickEnabled(true);
    dragSelect->PrimaryComponentTick.SetTickFunctionEnable(true);
    dragSelect->SetBoard(this);
    AddOwnedComponent(dragSelect);
    this->AddInstanceComponent(dragSelect);

    rotator = CreateDefaultSubobject<UCreatorRotator>(FName("Creator Rotator"));
    rotator->RegisterComponent();
    rotator->PrimaryComponentTick.bCanEverTick = true;
    rotator->SetComponentTickEnabled(true);
    rotator->SetBoard(this);
    rotator->SetDragSelect(dragSelect);
    AddOwnedComponent(rotator);
    this->AddInstanceComponent(rotator);
    
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
UClass* ACreatorBoard::TileClass() const { return ACreatorTile::StaticClass(); }
//119, 50, 17