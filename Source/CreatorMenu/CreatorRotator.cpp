#pragma once
#include "CreatorRotator.h"
#include "CreatorBoard.h"
#include "DragSelect.h"
#include "SelectionAngle.h"
#include "Kismet/GameplayStatics.h"

void UCreatorRotator::SetSelectionType(const ESelectionAngle _selectionType)
{
    if (selectionType == _selectionType)
        return;
    selectionType = _selectionType;
    increment = board->GetBoardShape() == EBoardShape::Hex
         ? 30 : board->GetBoardShape() == EBoardShape::Triangle
         ? 60 : 45;
    switch(selectionType)
    {
    case ESelectionAngle::Downhill: newRoll =  increment; break;
    case ESelectionAngle::Uphill:   newRoll = -increment; break;
    case ESelectionAngle::Flat:     newRoll = 0; break;
    default: throw std::invalid_argument("Invalid selection type");        
    }
    
    const auto pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    const FRotator rotation = FRotator(0, 0, newRoll);
    
    dragSelect->OnRotate();
    board->SetActorRotation(FRotator(newRoll, 0, 0));
    pawn->SetActorRotation(FRotator(0, -90, -newRoll));
}// 55, 29