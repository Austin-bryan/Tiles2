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
	UPROPERTY(EditAnywhere)
		float scale;
protected:
	// These get nulled instantly if they are UPROPERTY()'s
	// ReSharper disable CppUE4ProbableMemoryIssuesWithUObject
	UDragSelect* dragSelect;
	UCreatorRotator* rotator;
	UShortcutDetector* shortcutDetector;
	// ReSharper enable CppUE4ProbableMemoryIssuesWithUObject
private:
	int rand = 0;
};