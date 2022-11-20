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

    int x = 3;
    int f = 425;    
    float y = 23.69f;
    // LogV("swag"_f, x, y, f);

    Log("dooppppeee", "swag"_f, x, y, f, LogParams(FColor::Red, 30));
    Log("swag"_f, x, "swags", y, x, f, f, "dopeodopepdope", f, f, "dswagp", "swagds", "swaopsd", LogParams(FColor::Green, 10));
    Log(23, 1, location.ToString(), 23.01243f, f, LogParams(FColor::Black, 3));
    Log(23);
    Log(234, "sag", LogParams(FColor::White), "test"_f, 1211);
    Log(234, "sag", LogParams(FColor::Cyan), "test"_f, 1211);
    Log(location);
    Log(location + location);
    Log(location, FRotator(23, 2000, 1), "swag");

    Log(23, LogParams(FColor::Blue, 2));
    Log(location, LogParams(FColor::Green, 100));
    Log(LogParams(FColor::Purple), location + location);
    Log(location, FRotator(23, 2000, 1), LogParams(FColor::White), "swag");

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