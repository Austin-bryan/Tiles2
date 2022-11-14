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
    void NativeConstruct() override; 
    void NativePreConstruct() override;

    UPROPERTY(EditAnywhere)
        FVector2D Size;
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Default")
    void OnToggle();
};