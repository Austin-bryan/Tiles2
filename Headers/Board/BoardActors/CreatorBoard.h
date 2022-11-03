#pragma once
#include "CoreMinimal.h"
#include "Board.h"
#include "CreatorBoard.generated.h"

class UDragSelect;
class ATile;
enum class EBoardShape;

UCLASS()
class TILES2_API ACreatorBoard : public ABoard
{
	GENERATED_BODY()
public:
	void Tick(float DeltaSeconds) override;
	UClass* TileClass() const override;
	void DrawCircle(const FVector& Base, const FVector& X, const FVector& Y, FColor Color, float Radius, int32 NumSides, uint8 DepthPriority);
	void DrawBox(FVector worldPosition, bool shouldDeselect);
	ACreatorBoard();
	~ACreatorBoard();
protected:
	UPROPERTY()
	UDragSelect* dragSelect;
};