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

	void Tick(float DeltaSeconds) override;
	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Default")
		UCreatorRotator* GetCreatorRotator() const;
protected:
	// UPROPERTY()
		UShortcutDetector* shortcutDetector;
	// UPROPERTY()
		UDragSelect* dragSelect;
	// UPROPERTY()
		UCreatorRotator* rotator;
private:
	int rand = 0;
};