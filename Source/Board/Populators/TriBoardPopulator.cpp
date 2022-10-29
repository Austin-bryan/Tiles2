#include "TriBoardPopulator.h"
#include "Board.h"
#include "Enums.h"
#include "Tile.h"
#include "TriCoord.h"

void TriBoardPopulator::Populate(const FCoordPtr size, Tiles& tiles)
{
	/* size->X controls column loop, size->Z controls row loop, while size->Y doesn't control any loops
	 * startCoord positions itself in the topleft most tile. 

	 * on a diamond board, the lowest x bounds is the top left edge, highest x bounds is the bottom right edge
	 * vice versa for y bounds
	 * z is low at top and high at bottom
	 * 
	 * the check if x is greater than zero is used for when we are clipping the top corner off, 
	 * as x needs to start at a higher base value. the amount its higher is based on the distance between size->X and size->Z
	 * 
	 * the check if y is greater than size->X and size->Z is when we are clipping both x and z, but not y.
	 * this check is compnsanting for the fact that y doesn't control any loops, and is dependent on both X and Z
	 * 
	 */
	FTriCoord startCoord
	{
		ceil(size->X() / 2) - size->X() + 
			  (size->X() > size->Z() ? (size->X() - size->Z()) :
			  (size->Y() > size->X() && size->Y() > size->Z()) ? (size->Y() - size->X()) : 0),
		ceil(size->Y() / 2) - size->Y(),
		ceil(size->Z() / 2) - size->Z(),
		true
	};
	FTriCoord coord	= startCoord;

	for (int i = 0; i < size->Z(); i++)
		// Each loop goes through one row, left to right
	{
		while (coord.X() >= -GetHalf(size->X()))
			// Starts from top right, ends bottom left
		{
			const auto limit = size->X() - GetHalf(size->X()) - 1;

			while (coord.X() > limit)
				coord += EDirection::Right;
			if (coord.X() > limit)
				continue;
			if (coord.Y() > size->Y() - GetHalf(size->Y()) - 1)
				break;
			ATile* tile = CreateTile(MakeShared<FTriCoord>(coord.X(), coord.Y(), coord.Z(), coord.isUp), tiles);

			// Rotate tile accordingly
			if (!coord.GetIsUp())
			{
				tile->AddActorWorldRotation(FRotator(60, 0, 0));
				tile->AddActorWorldOffset(FVector(0, 0, coord.GetSpaceZ() / 3));
			}
			coord += EDirection::Right;
		}
		// Moves to next row starting position
		startCoord += EDirection::DownLeft;
		startCoord += EDirection::DownLeft;
		coord = startCoord;
	}

	// Positions the camera to be centered
	// todo:: add cam zoom out
	// TArray<FVector> positions;
	// TArray<ATile*> tileValue;
	//
	// tiles.GenerateValueArray(tileValue);
	// for (int i = 0; i < tiles.Num(); i++)
	// 	positions.Add(tileValue[i]->GetActorLocation());
	// FVector averagePos = UKismetMathLibrary::GetVectorArrayAverage(positions);
	//
	// APawn* pawn  = UGameplayStatics::GetPlayerPawn(board, 0);
	// averagePos.X = pawn->GetActorLocation().X;
	// pawn->SetActorLocation(averagePos);
}
