#pragma once
#include "CoreMinimal.h"
#include "Board.h"
#include "CreatorBoard.generated.h"

class UDragSelect;
class UCreatorRotator;
class UShortcutDetector;
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
	
	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Default")
		UCreatorRotator* GetCreatorRotator() const { return rotator; }
	UPROPERTY(EditInstanceOnly)
		UShortcutDetector* shortcutDetector;
protected:
	UPROPERTY(EditInstanceOnly)
		UDragSelect* dragSelect;
	UPROPERTY(EditInstanceOnly)
		UCreatorRotator* rotator;
private:
	int rand = 0;
};