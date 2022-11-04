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
	UClass* TileClass() const override;
	ACreatorBoard();
	~ACreatorBoard();
	
	UPROPERTY(EditAnywhere)
		float scale;
protected:
	UPROPERTY(VisibleAnywhere)
		UDragSelect* dragSelect;
};