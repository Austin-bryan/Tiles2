#pragma once
#include "CoreMinimal.h"
#include "Board.h"
#include "CreatorBoard.generated.h"

class UDragSelect;
class UCreatorRotator;
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

	UFUNCTION(BlueprintCallable, Category="Default")
		UCreatorRotator* GetCreatorRotator() const { return rotator; }
	UPROPERTY(EditAnywhere)
		float scale;

	// These get nulled instantly if they are UPROPERTY()'s
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	UDragSelect* dragSelect;
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	UCreatorRotator* rotator;
protected:
private:
	int rand = 0;
};