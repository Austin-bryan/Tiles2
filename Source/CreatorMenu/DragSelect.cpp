#pragma once
#include "DragSelect.h"
#include "CreatorBoard.h"
#include "SelectionBox.h"
#include "SelectionDrawer.h"
#include "Components/LineBatchComponent.h"
#include "Slate/SceneViewport.h"
#include "SelectionAngle.h"

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
    ChangeSelectionShape(ESelectionShape::Square);
}
void UDragSelect::TickComponent(const float deltaTime, const ELevelTick tickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(deltaTime, tickType, ThisTickFunction);

    const auto controller = GetBoard()->GetWorld()->GetFirstPlayerController();
    auto GetScreenToWorld = [this, controller]
    {
        float posX, posY;
        FVector _, worldPosition;
        
        controller->GetMousePosition(posX, posY);
        controller->DeprojectScreenPositionToWorld(posX, posY, worldPosition, _);
        return worldPosition;
    };

    if (anchorPoint.IsSet())
    {
        if (FVector::Distance(GetScreenToWorld(), *anchorPoint) < 10)
            selectionBox->SetActorLocation(FVector::Zero());
        else
        {
            if (controller->IsInputKeyDown(EKeys::LeftMouseButton))    
                drawer->Draw(GetScreenToWorld(), *anchorPoint);        
            else                                                       
            {                                                          
                lineBatchComponent->Flush();                           
                anchorPoint.Reset();                                   
                rotation = FRotator::ZeroRotator;                      
                selectionBox->SetVisibility(false);                    
            }                                                              
        }
    }
    else if (controller->IsInputKeyDown(EKeys::LeftMouseButton))
    {
        anchorPoint = GetScreenToWorld();
        selectionBox->SetVisibility(true);
    }
}
void UDragSelect::OnRotate() const { selectionBox->SetVisibility(false, true); }
void UDragSelect::ChangeSelectionShape(const ESelectionShape _shape)
{
    drawer = SelectionDrawer::Create(_shape, lineBatchComponent, selectionBox);
    drawer->SetCreatorBoard(GetBoard());
    shape = _shape; 
}