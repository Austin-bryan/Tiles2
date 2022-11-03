#include "CreatorBoard.h"
#include "CreatorTile.h"
#include "DragSelect.h"
#include "Kismet/GameplayStatics.h"

float ACreatorBoard::Scale;

void ACreatorBoard::DrawCircle(const FVector& Base, const FVector& X,const FVector& Y,FColor Color,float Radius,int32 NumSides, uint8 DepthPriority)
{
    // const float	AngleDelta = 2.0f * PI / NumSides;
    // FVector	LastVertex = Base + X * Radius;
    //
    // for(int32 SideIndex = 0; SideIndex < NumSides; SideIndex++)
    // {
    //     const FVector Vertex = Base + (X * FMath::Cos(AngleDelta * (SideIndex + 1)) + Y * FMath::Sin(AngleDelta * (SideIndex + 1))) * Radius;
    //     new(lineBatchComponent->BatchedLines) FBatchedLine(LastVertex,Vertex,Color,100, thickness, DepthPriority);
    //     LastVertex = Vertex;
    // }
    // lineBatchComponent->MarkRenderStateDirty();
}
void ACreatorBoard::DrawBox(FVector worldPosition, bool shouldDeselect)
{
// todo:: seperate this functino into the draw method and select method
// todo:: generalize this function to allow for triangle selet, pentagon select, and circular select
    
}

ACreatorBoard::ACreatorBoard()
{
    // lineBatchComponent = CreateDefaultSubobject<ULineBatchComponent>(FName("Line Component"));
    dragSelect = CreateDefaultSubobject<UDragSelect>(FName("Drag Select"));
    dragSelect->RegisterComponent();
    dragSelect->PrimaryComponentTick.bCanEverTick = true;
    dragSelect->SetComponentTickEnabled(true);
    dragSelect->PrimaryComponentTick.SetTickFunctionEnable(true);
    dragSelect->SetBoard(this);

    // auto d= GetWorld()->GetPlayerControllerIterator();
}
ACreatorBoard::~ACreatorBoard()
{
    ACreatorTile::EmptySelectedTiles();
}

void ACreatorBoard::Tick(const float DeltaSeconds)
{
    Scale = scale;
}
UClass* ACreatorBoard::TileClass() const { return ACreatorTile::StaticClass(); }
//119