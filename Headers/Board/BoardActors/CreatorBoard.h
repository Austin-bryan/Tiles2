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
protected:
	UShortcutDetector* shortcutDetector;
	UDragSelect* dragSelect;
	UCreatorRotator* rotator;
	UCreatorMenu* creatorMenu;
private:
	int rand = 0;
};