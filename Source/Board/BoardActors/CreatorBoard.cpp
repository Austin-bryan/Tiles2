#include "CreatorBoard.h"
#include "CreatorTile.h"
#include "Components/LineBatchComponent.h"
#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"
#include "TilesMap.h"

void ACreatorBoard::DrawCircle(const FVector& Base, const FVector& X,const FVector& Y,FColor Color,float Radius,int32 NumSides, uint8 DepthPriority)
{
    const float	AngleDelta = 2.0f * PI / NumSides;
    FVector	LastVertex = Base + X * Radius;

    for(int32 SideIndex = 0; SideIndex < NumSides; SideIndex++)
    {
        const FVector Vertex = Base + (X * FMath::Cos(AngleDelta * (SideIndex + 1)) + Y * FMath::Sin(AngleDelta * (SideIndex + 1))) * Radius;
        new(lineBatchComponent->BatchedLines) FBatchedLine(LastVertex,Vertex,Color,100, thickness, DepthPriority);
        LastVertex = Vertex;
    }
    lineBatchComponent->MarkRenderStateDirty();
}
void ACreatorBoard::DrawBox(FVector worldPosition)
{
    worldPosition.Y = firstClick->Y;

    TArray verts
    {
        *firstClick,   FVector(firstClick->X,   firstClick->Y, worldPosition.Z),
        worldPosition, FVector(worldPosition.X, firstClick->Y, firstClick->Z),
    };
    for (auto& vert : verts)
    {
        vert.Y = GetActorLocation().Y + 1;
        vert = GetActorTransform().TransformPosition(
             GetActorTransform().InverseTransformPosition(vert) * scale);
    }
    TArray<FBatchedLine> lines;
    for (int i = 0; i < verts.Num(); i++) 
        lines.Add(FBatchedLine(verts[i], verts[(i + 1) % verts.Num()],
            FColor::Red, 0, thickness, 0));

    lineBatchComponent->DrawLines(lines);
    TInterval<float> intervalZ, intervalX;
    
    auto SetMinMax = [](const float f1, const float f2, TInterval<float>& interval) {
        interval = f1 > f2 ? TInterval(f2, f1) : TInterval(f1, f2); };
    SetMinMax(verts[0].Z, verts[2].Z, intervalZ);
    SetMinMax(verts[0].X, verts[2].X, intervalX);

    for (const auto& tile : tiles.Values())
    {
        const auto creatorTile = static_cast<ACreatorTile*>(tile);

        if (const auto loc = creatorTile->GetActorLocation();
            intervalZ.Contains(loc.Z) && intervalX.Contains(loc.X))
                creatorTile->Select();
        else creatorTile->Deselect();
    }
}

ACreatorBoard::ACreatorBoard()
{
    lineBatchComponent = CreateDefaultSubobject<ULineBatchComponent>(FName("Line Component"));
}

void ACreatorBoard::Tick(const float DeltaSeconds)
{
    lineBatchComponent->Flush();
    Super::Tick(DeltaSeconds);

    const auto controller = GetWorld()->GetFirstPlayerController();
    auto GetScreenToWorld = [this, controller]
    {
        float posX, posY;
        FVector _, worldPosition;
        
        controller->GetMousePosition(posX, posY);
        controller->DeprojectScreenPositionToWorld(posX, posY, worldPosition, _);
        return worldPosition;
    };

    if (firstClick.IsSet())
    {
        if (controller->IsInputKeyDown(EKeys::LeftMouseButton))
             DrawBox(GetScreenToWorld());
        else firstClick.Reset();
    }
    else if (controller->IsInputKeyDown(EKeys::LeftMouseButton))
        firstClick = GetScreenToWorld();
}
UClass* ACreatorBoard::TileClass() const { return ACreatorTile::StaticClass(); }
//119