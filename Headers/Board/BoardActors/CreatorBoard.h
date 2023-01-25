#pragma once
#include "CoreMinimal.h"
#include "Board.h"
#include "Clipboard.h"
#include "CreatorBoard.generated.h"

class ASeedCreator;
class ATile;
class UCreatorMenu;
class UDragSelect;
class UCreatorRotator;
class UShortcutDetector;
enum class EBoardShape;

UCLASS()
class TILES2_API ACreatorBoard : public ABoard
{
	GENERATED_BODY()
public:
	UClass* TileClass() const override;
	void Tick(float DeltaSeconds) override;
	void BeginPlay() override;
	ACreatorBoard();
	Clipboard& GetClipboard() { return clipboard.GetValue(); }
protected:
	UFUNCTION(BlueprintCallable, Category="Default")
	UDragSelect* GetDragSelect() const;

	UFUNCTION(BlueprintCallable, Category="Default")
	UCreatorRotator* GetCreatorRotator() const;

	UFUNCTION(BlueprintCallable, Category="Default")
	void SetCreatorMenu(UCreatorMenu* _creatorMenu);

	UPROPERTY() UShortcutDetector* shortcutDetector;
	UPROPERTY() UDragSelect* dragSelect;
	UPROPERTY() UCreatorRotator* rotator;
	UPROPERTY() UCreatorMenu* creatorMenu;
	UPROPERTY() ASeedCreator* SeedCreator;
private:
	TOptional<Clipboard> clipboard;
};