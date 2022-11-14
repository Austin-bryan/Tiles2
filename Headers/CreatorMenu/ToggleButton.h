#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ToggleButton.generated.h"

UCLASS()
class UToggleButton : public UUserWidget
{
    GENERATED_BODY()
public:
    void NativeConstruct() override; 
    void NativePreConstruct() override; 
};