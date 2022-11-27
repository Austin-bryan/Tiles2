#pragma once
#include "CoreMinimal.h"
#include "Board.h"
#include "CreatorBoard.generated.h"

class UDragSelect;
class UCreatorRotator;
class UShortcutDetector;
class ATile;
class UCreatorMenu;
enum class EBoardShape;

UENUM()
enum class EVertexMode : uint8
{
	PrevPrev, PrevNext, NextNext, NextPrev
};

UCLASS()
class TILES2_API ACreatorBoard : public ABoard
{
	GENERATED_BODY()
public:
	UClass* TileClass() const override;
	ACreatorBoard();
	~ACreatorBoard();

	void BeginPlay() override;

	// todo:: can these be protected?
	UFUNCTION(BlueprintCallable, Category="Default")
		UDragSelect* GetDragSelect() const;
	UFUNCTION(BlueprintCallable, Category="Default")
		UCreatorRotator* GetCreatorRotator() const;
	UFUNCTION(BlueprintCallable, Category="Default")
		void SetCreatorMenu(UCreatorMenu* _creatorMenu);
	UPROPERTY(EditAnywhere)
		EVertexMode VertexMode;
protected:
	UShortcutDetector* shortcutDetector;
	UDragSelect* dragSelect;
	UCreatorRotator* rotator;
	UCreatorMenu* creatorMenu;
private:
	int rand = 0;
};