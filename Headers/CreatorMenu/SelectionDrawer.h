#pragma once
#include "CoreMinimal.h"
#include "AssetDir.h"

enum class ESelectionType : uint8;
struct FBatchedLine;
class ASelectionBox;
class ULineBatchComponent;

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
    virtual int AngleOffset() = 0;
    
    ASelectionBox* selectionBox;
    ULineBatchComponent* lineBatchComponent;
    const float angleMultiplier = 57.25f;
    const float thickness = 4;
private:
    void Init();
};

class TILES2_API CenterSelection : public SelectionDrawer
{
public:
    CenterSelection(ULineBatchComponent* lineBatch, ASelectionBox* box)
        : SelectionDrawer{ lineBatch, box } { }
private:
    void GetVerts(TArray<FVector>& verts, FVector&& worldPosition, FVector anchorPoint) override;
};

class TILES2_API SquareSelection : public SelectionDrawer
{
public:
    SquareSelection(ULineBatchComponent* lineBatch, ASelectionBox* box)
        : SelectionDrawer{ lineBatch, box } { }
    void GetVerts(TArray<FVector>& verts, FVector&& worldPosition, FVector anchorPoint) override;
private:
    int VertexCount() override { return 4; }
    int AngleOffset() override { return 45; }
    FString Mesh() override    { return SIMPLE_SQUARE; }
};

class TILES2_API TriangleSelection : public CenterSelection
{
public:
    TriangleSelection(ULineBatchComponent* lineBatch, ASelectionBox* box)
        : CenterSelection{ lineBatch, box } { }
private:
    int VertexCount() override { return 3; }
    int AngleOffset() override { return 60; }
    FString Mesh() override    { return ""; }
};

class TILES2_API CircleSelection : public CenterSelection
{
public:
    CircleSelection(ULineBatchComponent* lineBatch, ASelectionBox* box)
        : CenterSelection{ lineBatch, box } { }
private:
    int VertexCount() override { return 128; }
    int AngleOffset() override { return 0; }
    FString Mesh() override    { return SELECTION_CYLINDER; }
};

