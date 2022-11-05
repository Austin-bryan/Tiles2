#pragma once
#include "DragSelect.h"
#include "Components/LineBatchComponent.h"
#include "GameFramework/HUD.h"
#include "CreatorBoard.h"
#include "SAdvancedTransformInputBox.h"
#include "SegmentTypes.h"
#include "SelectionBox.h"
#include "Kismet/GameplayStatics.h"

UDragSelect::UDragSelect() 
{
    UActorComponent::SetComponentTickEnabled(true);
    lineBatchComponent = CreateDefaultSubobject<ULineBatchComponent>(FName("Line Batch"));
}

void UDragSelect::BeginPlay()
{
    selectionBox = GetWorld()->SpawnActor<ASelectionBox>(
        ASelectionBox::StaticClass(),
        FVector::Zero(),
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
            Draw(GetScreenToWorld());
        else
        {
            lineBatchComponent->Flush();
            firstClick.Reset();
            rotation = FRotator::ZeroRotator;
            selectionBox->SetVisibility(false);
        }
    }
    else if (controller->IsInputKeyDown(EKeys::LeftMouseButton))
    {
        firstClick = GetScreenToWorld();
        selectionBox->SetVisibility(true);
    }
}
void UDragSelect::Draw(FVector&& worldPosition)
{
    lineBatchComponent->Flush();
    worldPosition.Y = firstClick->Y;

    TArray verts
    {
        *firstClick,
    	FVector(worldPosition.X, firstClick->Y, firstClick->Z),
        worldPosition,
        FVector(firstClick->X, firstClick->Y, worldPosition.Z),
    };

    const FVector avgPos = (verts[0] + verts[2]) / 2;
    const float height   = FVector::Distance(verts[1], verts[2]);
    const float width    = FVector::Distance(verts[0], verts[1]);

    selectionBox->SetActorLocation(avgPos);
    selectionBox->ScaleArea(width, height);

    TArray<FBatchedLine> lines;
    for (int i = 0; i < verts.Num(); i++) 
        lines.Add(FBatchedLine(verts[i], verts[(i + 1) % verts.Num()],
            FLinearColor(0, 0, 0, 0.6f), 0, thickness, 0));
    lineBatchComponent->DrawLines(lines);
}