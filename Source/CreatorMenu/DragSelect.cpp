#pragma once
#include "DragSelect.h"
#include "Components/LineBatchComponent.h"
#include "GameFramework/HUD.h"
#include "CreatorBoard.h"
#include "SAdvancedTransformInputBox.h"
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
        if (controller->IsInputKeyDown(EKeys::LeftAlt))
        {
            secondClick = GetScreenToWorld();
        }
        if (controller->IsInputKeyDown(EKeys::LeftMouseButton))
        {
            FVector vert1;
            Draw(GetScreenToWorld(), vert1);
            
            if (controller->IsInputKeyDown(EKeys::R))
            {
                rotation = (*firstClick - GetScreenToWorld()).Rotation() ;
                rotation = FRotator(rotation.Pitch, 0, 0);
                Log(rotation);
            }
        }
        else
        {
            lineBatchComponent->Flush();
            firstClick.Reset();
            secondClick.Reset();
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
void UDragSelect::Draw(FVector&& worldPosition, FVector& vert1)
{
    lineBatchComponent->Flush();
    worldPosition.Y = firstClick->Y;


    //todo:: the trapazoid effect happens when secondclick is lockded down then the mouse is moved
    //todo: selection must always map to mouse
    //todo: if second click could be updated to stay perpendicular to worldPosition, vert4 could get
    //todo: calculated to be at the correct position as well
    //todo: to do this, it might require caching last frames verts 2 and 3.
    //todo:: factor verts into its own nested class
    TArray verts
    {
        *firstClick,
        secondClick ? *secondClick : FVector(firstClick->X,   firstClick->Y, worldPosition.Z),
        worldPosition,
        secondClick
            ? FVector(*firstClick - *secondClick + worldPosition)
            : FVector(worldPosition.X, firstClick->Y, firstClick->Z),
    };
    vert1 = verts[3];

    const FTransform trans(*firstClick);
    for (auto& vert : verts)
    {
        vert = trans.TransformPosition(
            rotation.RotateVector(
                trans.InverseTransformPosition(vert)));
    }

    const FVector avgPos = (verts[0] + verts[2]) / 2;
    const float height   = FVector::Distance(verts[0], verts[1]);
    const float width    = FVector::Distance(verts[1], verts[2]);

    selectionBox->SetActorLocation(avgPos);
    selectionBox->ScaleArea(width, height);
    selectionBox->SetActorRotation(FRotator(0, 90, -rotation.Pitch));

    TArray<FBatchedLine> lines;
    for (int i = 0; i < verts.Num(); i++) 
        lines.Add(FBatchedLine(verts[i], verts[(i + 1) % verts.Num()],
            FLinearColor(0, 0, 0, 0.6f), 0, thickness, 0));
    lineBatchComponent->DrawLines(lines);
}
void UDragSelect::SetBoard(ACreatorBoard* _board) { board = _board; }