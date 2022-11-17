#pragma once
#include "SelectionDrawer.h"

#include "CreatorBoard.h"
#include "Enums.h"
#include "SelectionBox.h"
#include "SelectionAngle.h"
#include "Components/LineBatchComponent.h"

SelectionDrawer::SelectionDrawer(ULineBatchComponent* lineBatch, ASelectionBox* box)
{
    lineBatchComponent = lineBatch, selectionBox = box;
}
void SelectionDrawer::Init()
{
    selectionBox->SetMesh(Mesh());
}

TUniquePtr<SelectionDrawer> SelectionDrawer::Create(const ESelectionShape mode, ULineBatchComponent* lineBatch, ASelectionBox* box)
{
    // TODO:: Handle angle cases
    TUniquePtr<SelectionDrawer> drawer;
    
    switch(mode)
    {
    case ESelectionShape::Square:   drawer = MakeUnique<SquareSelection>(lineBatch, box); break;
    case ESelectionShape::Circle:   drawer = MakeUnique<CircleSelection>(lineBatch, box); break;
    case ESelectionShape::Triangle: drawer = MakeUnique<TriangleSelection>(lineBatch, box); break;
    default: ;
    }
    drawer->Init();
    return drawer;
}

void SelectionDrawer::Draw(FVector&& worldPosition, const  FVector anchorPoint)
{
    lineBatchComponent->Flush();
    worldPosition.Y = anchorPoint.Y;        // Makes sure we're working in the correct plane

    TArray<FVector> verts;
    GetVerts(verts, std::move(worldPosition), anchorPoint);

    // Create lines to be drawn later
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
    const int rollOffset = RollOffset(anchorTrans, worldPosition);

    // Create the Circle or Triangle
    for (int i = 0; i < VertexCount(); i++)
    {
        FRotator rotator{ delta * i * angleMultiplier + rollOffset, 0, 0 };
        const auto result = anchorTrans.TransformPosition(rotator.RotateVector(edge));
        verts.Add(result);
    }
    selectionBox->SetActorLocation(anchorPoint);
    selectionBox->ScaleArea(radius * Scale(), radius * Scale());
    selectionBox->SetActorRotation(FRotator(0, 90, rollOffset));
}

int TriangleSelection::RollOffset(const FTransform& anchorTrans, const FVector& worldPosition)
{
    // Uses a normalized worldPosition relative to anchor to get the angle
    auto inverseWorldPosition = anchorTrans.InverseTransformPosition(worldPosition);
    inverseWorldPosition.Normalize();

    const auto rad = atan2(inverseWorldPosition.Z - 0, inverseWorldPosition.X - 0);
    float deg = rad * (180 / PI) + 180 - 30;

    // Snaps if less than the snap limit
    constexpr int snapLimit = 20;
    const auto SnapAngle = [this, snapLimit](float& degree, const int target)
    {
        const int mod = static_cast<int>(degree) % 60;
        const int abs = FMath::Abs(mod);

        // Different offset depending on board
        const bool result = board->GetBoardShape() == EBoardShape::Hex
            ? abs > 20 && abs < 40
            : abs < 10 || abs > 50;

        float d = degree;

        if (result)
        {
            const float normalized = degree / 30;
            const float rounded = FMath::RoundToFloat(normalized);
            const float final = rounded * 30;
    
            degree = final;
        }
        return result;
    };

    if (!SnapAngle(deg, 240))
        SnapAngle(deg, 60);
    return deg;
}

SquareSelection::SquareSelection(ULineBatchComponent* lineBatch, ASelectionBox* box): SelectionDrawer{ lineBatch, box }
{
    // Clears the roll, incase Triangle was used last
    selectionBox->SetActorRotation(FRotator(0, 90, 0));
}
void SquareSelection::GetVerts(TArray<FVector>& verts, FVector&& worldPosition, const FVector anchorPoint)
{
    lineBatchComponent->Flush();
    worldPosition.Y = anchorPoint.Y;

    // Anchor point never moves, World Position is where the cursor is.
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
}