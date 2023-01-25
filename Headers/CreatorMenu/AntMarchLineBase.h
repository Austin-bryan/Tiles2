#pragma once
#include "CoreMinimal.h"
#include "AntMarchLineBase.generated.h"

class UAntMarchSpawner;
class UNiagaraComponent;

UCLASS()
class AAntMarchLineBase : public AActor
{
    GENERATED_BODY()
public:
    AAntMarchLineBase();
    void SetEndPosition(FVector endPosition);
    void Stop() const;
protected:
    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent* Root;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* StaticMesh;

    UPROPERTY()
    UMaterialInstanceDynamic* DynamicMaterial;
private:
    float time;
};