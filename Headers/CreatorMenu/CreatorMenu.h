#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreatorMenu.generated.h"

class UButton;
class UToggleButton;
enum class ESelectionAngle : uint8;
enum class ETileColor : uint8;

UCLASS()
class UCreatorMenu : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, Category="Default")
        UToggleButton* GetAngleButton(ESelectionAngle selectionAngle);
    UFUNCTION(BlueprintNativeEvent, Category="Default")
        UToggleButton* GetColorButton(ETileColor color);
};