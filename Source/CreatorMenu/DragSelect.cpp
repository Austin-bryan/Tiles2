#pragma once
#include "DragSelect.h"

#include <ThirdParty/SPIRV-Reflect/SPIRV-Reflect/include/spirv/unified1/spirv.h>

#include "CreatorTile.h"
#include "Components/LineBatchComponent.h"
#include "GameFramework/HUD.h"
#include "CreatorBoard.h"
#include "SelectionBox.h"
#include "Kismet/GameplayStatics.h"

void UDragSelect::Select() {}

UDragSelect::UDragSelect() 
{
    UActorComponent::SetComponentTickEnabled(true);
    lineBatchComponent = CreateDefaultSubobject<ULineBatchComponent>(FName("Line Batch"));
}

void UDragSelect::BeginPlay()
{
    Log("create box");
    selectionBox = GetWorld()->SpawnActor<ASelectionBox>(
        ASelectionBox::StaticClass(),
        board->GetActorLocation(),
        FRotator(0, 90, 0));
    Super::BeginPlay();
}

void UDragSelect::TickComponent(const float deltaTime, const ELevelTick tickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(deltaTime, tickType, ThisTickFunction);

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
            TArray<FBatchedLine> lines;

            Draw(lines, GetScreenToWorld());
            Select(lines,!controller->IsInputKeyDown(EKeys::LeftShift));

        }
        else
        {
            lineBatchComponent->Flush();
            firstClick.Reset();
            selectionBox->SetVisibility(false);
        }
    }
    else if (controller->IsInputKeyDown(EKeys::LeftMouseButton))
    {
        firstClick = GetScreenToWorld();
        selectionBox->SetVisibility(true);
    }
}
void UDragSelect::Draw(TArray<FBatchedLine>& lines, FVector&& worldPosition)
{
    lineBatchComponent->Flush();
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

    const FVector avgPos = (verts[0] + verts[2]) / 2;
    const float height   = FVector::Distance(verts[0], verts[1]);
    const float width    = FVector::Distance(verts[1], verts[2]);

    selectionBox->SetActorLocation(avgPos);
    selectionBox->SetActorScale3D(FVector(0.001, width, height) / 100);

    for (int i = 0; i < verts.Num(); i++) 
        lines.Add(FBatchedLine(verts[i], verts[(i + 1) % verts.Num()],
            FLinearColor(0, 0, 0, 0.6f), 0, thickness, 0));
    lineBatchComponent->DrawLines(lines);
}
void UDragSelect::Select(const TArray<FBatchedLine>& lines, const bool shouldDeselect)
{
    TInterval<float> intervalZ, intervalX;

    auto SetMinMax = [](const float f1, const float f2, TInterval<float>& interval) {
        interval = f1 > f2 ? TInterval(f2, f1) : TInterval(f1, f2); };

    SetMinMax(lines[0].Start.Z, lines[2].Start.Z, intervalZ);
    SetMinMax(lines[0].Start.X, lines[2].Start.X, intervalX);
    
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

