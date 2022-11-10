#pragma once
#include "SelectionDrawer.h"
#include "SelectionBox.h"
#include "SelectionType.h"
#include "Components/LineBatchComponent.h"

SelectionDrawer::SelectionDrawer(ULineBatchComponent* lineBatch, ASelectionBox* box)
{
    lineBatchComponent = lineBatch, selectionBox = box;
}
void SelectionDrawer::Init()
{
    selectionBox->SetMesh(Mesh());
}

TUniquePtr<SelectionDrawer> SelectionDrawer::Create(const ESelectionType mode, ULineBatchComponent* lineBatch, ASelectionBox* box)
{
    // TODO:: Handle angle cases
    TUniquePtr<SelectionDrawer> drawer;
    
    switch(mode)
    {
    case ESelectionType::Square:   drawer = MakeUnique<SquareSelection>(lineBatch, box); break;
    case ESelectionType::Circle:   drawer = MakeUnique<CircleSelection>(lineBatch, box); break;
    case ESelectionType::Triangle: drawer = MakeUnique<TriangleSelection>(lineBatch, box); break;
    default: ;
    }
    drawer->Init();
    return drawer;
}

void SelectionDrawer::Draw(FVector&& worldPosition, const  FVector anchorPoint)
{
    lineBatchComponent->Flush();
    worldPosition.Y = anchorPoint.Y;

    TArray<FVector> verts;
    GetVerts(verts, std::move(worldPosition), anchorPoint);

    TArray<FBatchedLine> lines;
    for (int i = 0; i < verts.Num(); i++) 
        lines.Add(FBatchedLine(verts[i], verts[(i + 1) % verts.Num()],
            FLinearColor(0, 0, 0, 0.6f), 0, thickness, 0));
    lineBatchComponent->DrawLines(lines);
}

void CenterSelection::GetVerts(TArray<FVector>& verts, FVector&& worldPosition, const FVector anchorPoint)
{
    const double radius = FVector::Distance(anchorPoint, worldPosition);
    const float delta = 2.0f * PI / VertexCount();
    
    const FTransform anchorTrans{ anchorPoint };
    const FVector edge = FVector(0, 0, radius);
    
    for (int i = 0; i < VertexCount(); i++)
    {
        FRotator rotator{ delta * i * angleMultiplier + 45, 0, 0 };
        const auto result = anchorTrans.TransformPosition(rotator.RotateVector(edge));
        verts.Add(result);
    }
    selectionBox->SetActorLocation(anchorPoint);
    selectionBox->ScaleArea(radius * 2, radius * 2);
}
void SquareSelection::GetVerts(TArray<FVector>& verts, FVector&& worldPosition, const FVector anchorPoint)
{
    lineBatchComponent->Flush();
    worldPosition.Y = anchorPoint.Y;
    
    verts =
    {
        anchorPoint,
        FVector(worldPosition.X, anchorPoint.Y, anchorPoint.Z),
        worldPosition,
        FVector(anchorPoint.X, anchorPoint.Y, worldPosition.Z),
    };
    
    const FVector avgPos = (verts[0] + verts[2]) / 2;
    const float height   = FVector::Distance(verts[1], verts[2]);
    const float width    = FVector::Distance(verts[0], verts[1]);
    
    selectionBox->SetActorLocation(avgPos);
    selectionBox->ScaleArea(width, height);
    
    TArray<FBatchedLine> lines;
    for (int i = 0; i < verts.Num(); i++) 
        lines.Add(FBatchedLine(verts[i], verts[(i + 1) % verts.Num()],
            FLinearColor(0, 0, 0, 0.6f), 0, thickness, 0));
    lineBatchComponent->DrawLines(lines);
}