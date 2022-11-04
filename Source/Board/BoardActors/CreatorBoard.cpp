#include "CreatorBoard.h"
#include "CreatorTile.h"
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
}
ACreatorBoard::~ACreatorBoard() { ACreatorTile::EmptySelectedTiles(); }
UClass* ACreatorBoard::TileClass() const { return ACreatorTile::StaticClass(); }
//119, 50, 17