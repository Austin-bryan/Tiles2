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
		UCreatorRotator* GetCreatorRotator() const { return rotator; }
	UPROPERTY(EditInstanceOnly)
		UShortcutDetector* shortcutDetector;
	static float TriRot;
protected:
	UPROPERTY(EditInstanceOnly)
		UDragSelect* dragSelect;
	UPROPERTY(EditInstanceOnly)
		UCreatorRotator* rotator;
	UPROPERTY(EditInstanceOnly)
		float triangleRot = 1;
private:
	int rand = 0;
};