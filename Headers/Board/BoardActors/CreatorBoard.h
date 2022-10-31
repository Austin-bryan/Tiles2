#pragma once
#include "CoreMinimal.h"
#include "Board.h"
#include "CreatorBoard.generated.h"

class ATile;
enum class EBoardShape;

UCLASS()
class TILES2_API ACreatorBoard : public ABoard
{
	GENERATED_BODY()
	void Tick(float DeltaSeconds) override;
	UClass* TileClass() const override;
	void DrawCircle(const FVector& Base, const FVector& X, const FVector& Y, FColor Color, float Radius, int32 NumSides, uint8 DepthPriority);
	ACreatorBoard();
protected:
	UPROPERTY()
	ULineBatchComponent* lineBatchComponent;

	UPROPERTY(EditAnywhere)
	float thickness = 5;

	UPROPERTY(EditAnywhere)
	float scale = 57;

	TOptional<FVector> firstClick;
};