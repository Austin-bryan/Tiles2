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
protected:
    UPROPERTY()
    UStaticMeshComponent* mesh;
};