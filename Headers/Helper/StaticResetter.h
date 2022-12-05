#pragma once
#include "CoreMinimal.h"
#include "StaticResetter.generated.h"

UCLASS()
class AStaticResetter : public AActor
{
    GENERATED_BODY()
public:
    void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};

