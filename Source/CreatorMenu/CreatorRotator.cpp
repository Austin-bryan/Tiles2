#pragma once
#include "CreatorRotator.h"
#include "CreatorBoard.h"
#include "SelectionType.h"
#include "Kismet/GameplayStatics.h"

UCreatorRotator::UCreatorRotator()
{
    // InputComponent = CreateDefaultSubobject<UInputComponent>(FName("Input Component"));
}
void UCreatorRotator::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    if (!board)
        return;
    increment = board->GetBoardShape() == EBoardShape::Hex
         ? 30 : board->GetBoardShape() == EBoardShape::Triangle
         ? 60 : 45;
    constexpr float timeLength = 0.01f;
    const auto controller = GetWorld()->GetFirstPlayerController();

    if (controller->IsInputKeyDown(EKeys::R))
    {
        if (!rDown)
        {
            rDown = true;
            newRoll += increment;
        }
    }
    else rDown = false;
    if (controller->IsInputKeyDown(EKeys::L))
    {
        if (!lDown)
        {
            lDown = true;
            newRoll -= increment;
        }
    }
    else lDown = false;

    const auto pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    const FRotator rotation = FRotator(0, 0, newRoll);
    board->SetActorRotation(FRotator(newRoll, 0, 0));
    pawn->SetActorRotation(FRotator(0, -90, -newRoll));
}

void UCreatorRotator::SetSelectionType(const ESelectionType _selectionType)
{
    if (selectionType == _selectionType)
        return;
    selectionType = _selectionType;
    increment = board->GetBoardShape() == EBoardShape::Hex
         ? 30 : board->GetBoardShape() == EBoardShape::Triangle
         ? 60 : 45;

    switch(selectionType)
    {
    case ESelectionType::Downhill: newRoll =  increment; break;
    case ESelectionType::Uphill:   newRoll = -increment; break;
    case ESelectionType::Flat:     newRoll = 0; break;
    default: throw std::invalid_argument("Invalid selection type");        
    }
    
    const auto pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    const FRotator rotation = FRotator(0, 0, newRoll);
    board->SetActorRotation(FRotator(newRoll, 0, 0));
    pawn->SetActorRotation(FRotator(0, -90, -newRoll));
}

void UCreatorRotator::ResetRotation() const
{
    
}
