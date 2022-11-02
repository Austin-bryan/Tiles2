#pragma once
#include "DragSelect.h"
#include "CreatorTile.h"
#include "Components/LineBatchComponent.h"
#include "GameFramework/HUD.h"
#include "CreatorBoard.h"
#include "Kismet/GameplayStatics.h"

void UDragSelect::Select() {}

UDragSelect::UDragSelect()
{
    UActorComponent::SetComponentTickEnabled(true);
    lineBatchComponent = CreateDefaultSubobject<ULineBatchComponent>(FName("Line Batch"));
}

void UDragSelect::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    lineBatchComponent->Flush();

    const auto controller = board->GetWorld()->GetFirstPlayerController();
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
        if (FVector::Distance(GetScreenToWorld(), *firstClick) < 0.25f)
            return;
        if (controller->IsInputKeyDown(EKeys::LeftMouseButton))
        {
            Draw(GetScreenToWorld(), !controller->IsInputKeyDown(EKeys::LeftShift));
        }
        else firstClick.Reset();
    }
    else if (controller->IsInputKeyDown(EKeys::LeftMouseButton))
        firstClick = GetScreenToWorld();
}
void UDragSelect::Draw(FVector&& worldPosition, bool shouldDeselect)
{
    worldPosition.Y = firstClick->Y;
    
    TArray verts
    {
        *firstClick,   FVector(firstClick->X,   firstClick->Y, worldPosition.Z),
        worldPosition, FVector(worldPosition.X, firstClick->Y, firstClick->Z),
    };
    for (auto& vert : verts)
    {
        vert.Y = board->GetActorLocation().Y + 1;
        vert = board->GetActorTransform().TransformPosition(
             board->GetActorTransform().InverseTransformPosition(vert) * scale);
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
    
    for (const auto& tile : board->GetTiles().Values())
    {
        const auto creatorTile = static_cast<ACreatorTile*>(tile);
    
        if (const auto loc = creatorTile->GetActorLocation();
            intervalZ.Contains(loc.Z) && intervalX.Contains(loc.X))
                creatorTile->Select(true);
        else if (shouldDeselect) creatorTile->Deselect();
    }
}

void UDragSelect::SetBoard(ACreatorBoard* _board) { board = _board; }

