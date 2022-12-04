#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ToggleButton.generated.h"

class UButton;

UCLASS()
class UToggleButton : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Default")
        void OnToggle();
};