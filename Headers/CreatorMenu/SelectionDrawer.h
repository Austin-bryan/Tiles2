#pragma once
#include "CoreMinimal.h"
#include "AssetDir.h"

enum class ESelectionType : uint8;
struct FBatchedLine;
class ASelectionBox;
class ULineBatchComponent;

/**
 * Draws the outline of the selection box, and scales the body itself
 */
class TILES2_API SelectionDrawer
{
public:
    static TUniquePtr<SelectionDrawer> Create(ESelectionType mode, ULineBatchComponent* lineBatch, ASelectionBox* box);

    virtual ~SelectionDrawer(){ }
    void Draw(FVector&& worldPosition, FVector anchorPoint);
    virtual void GetVerts(TArray<FVector>& verts, FVector&& worldPosition, FVector anchorPoint) = 0;
    virtual FString Mesh() = 0;
protected:
    SelectionDrawer(ULineBatchComponent* lineBatch, ASelectionBox* box);
    virtual int VertexCount() = 0;
    virtual int RollOffset(const FTransform& anchorTrans, const FVector& worldPosition) = 0;
    
    ASelectionBox* selectionBox;
    ULineBatchComponent* lineBatchComponent;
    const float angleMultiplier = 57.25f;
    const float thickness = 4;
private:
    void Init();
};

class TILES2_API SquareSelection : public SelectionDrawer
{
public:
    SquareSelection(ULineBatchComponent* lineBatch, ASelectionBox* box);
    void GetVerts(TArray<FVector>& verts, FVector&& worldPosition, FVector anchorPoint) override;
private:
    int VertexCount() override { return 4; }
    int RollOffset(const FTransform& anchorTrans, const FVector& worldPosition) override { return 45; }
    FString Mesh() override    { return SIMPLE_SQUARE; }
};

/**
 * Parent class of TriangeSelection and CircleSelection.
 * Unlike SquareSelection, this will always stretch from the center outwards, remaining proptional.
 */
class TILES2_API CenterSelection : public SelectionDrawer
{
public:
    CenterSelection(ULineBatchComponent* lineBatch, ASelectionBox* box)
        : SelectionDrawer{ lineBatch, box } { }
protected:
    virtual float Scale() const = 0;
private:
    void GetVerts(TArray<FVector>& verts, FVector&& worldPosition, FVector anchorPoint) override;
};

class TILES2_API TriangleSelection : public CenterSelection
{
public:
    TriangleSelection(ULineBatchComponent* lineBatch, ASelectionBox* box)
        : CenterSelection{ lineBatch, box } { }
private:
    int RollOffset(const FTransform& anchorTrans, const FVector& worldPosition) override;
    int VertexCount() override   { return 3; }
    float Scale() const override { return 1.48f; } 
    FString Mesh() override      { return TRIANGLE_TILE; }
};
class TILES2_API CircleSelection : public CenterSelection
{
public:
    CircleSelection(ULineBatchComponent* lineBatch, ASelectionBox* box)
        : CenterSelection{ lineBatch, box } { }
private:
    int RollOffset(const FTransform& anchorTrans, const FVector& worldPosition) override { return 0; }
    int VertexCount() override   { return 128; }
    float Scale() const override { return 2.0f; } 
    FString Mesh() override      { return SELECTION_CYLINDER; }
};
