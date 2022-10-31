#include "BoardCoordinator.h"
#include "Board.h"
#include "PlayerBoard.h"
#include "CreatorBoard.h"
#include "SizeBoard.h"

void UBoardCoordinator::BeginPlay() { Super::BeginPlay(); }
void UBoardCoordinator::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBoardCoordinator::SetBoard(ABoard* board, const EBoardType boardType)
{
	switch (boardType)
	{
	case EBoardType::PlayerBoard:  if (!playerBoard)  playerBoard  = static_cast<APlayerBoard*> (board); break;
	case EBoardType::CreatorBoard: if (!creatorBoard) creatorBoard = static_cast<ACreatorBoard*>(board); break;
	case EBoardType::SizeBoard:    if (!sizeBoard)    sizeBoard    = static_cast<ASizeBoard*>   (board); break;
	case EBoardType::TargetBoard:  throw std::invalid_argument("Not yet implemented.");
	default: throw std::invalid_argument("Invalid board type.") ;
	}
}
