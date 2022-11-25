#pragma once
#include "CoreMinimal.h"
#include "Vertex.generated.h"

class UMeshGenerator;

UCLASS()
class TILES2_API AVertex : public AActor
{
    GENERATED_BODY()
public:
    void Init(
        const int _vertexIndex,
        const int _sideCount,
        const FVector _position,
        UMeshGenerator* _generator);
    
    bool IsMerged() const;
    FVector GetWorldPosition() const;
    FVector GetLocalPosition() const;
    AVertex* NextVertex() const;
    AVertex* PrevVertex() const;

    void ApplyPosition();
    void QueuePosition(const FVector newPosition);
    void SetPosition(const FVector newPosition);
protected:
    
private:
    int vertexIndex, sideCount;
    UMeshGenerator* generator;
    FVector position, queuedPosition;
    bool hasBeenMerged = false;
};

inline bool operator==(
    const AVertex& rhs,
    const AVertex& lhs)
{
    return rhs.GetWorldPosition() == lhs.GetWorldPosition();
}