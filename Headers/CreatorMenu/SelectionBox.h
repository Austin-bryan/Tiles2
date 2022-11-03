#pragma once
#include "CoreMinimal.h"
#include "SelectionBox.generated.h"

UCLASS()
class ASelectionBox : public AActor
{
    GENERATED_BODY()
public:
    ASelectionBox();
    void BeginPlay() override;
    void SetVisibility(bool visibility);
protected:
    UPROPERTY()
    UStaticMeshComponent* mesh;
private:
    bool isVisible;
};