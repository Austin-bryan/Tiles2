#include "CreatorBoard.h"
#include "CreatorTile.h"
#include "Components/LineBatchComponent.h"
#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"

void ACreatorBoard::DrawCircle(const FVector& Base, const FVector& X,const FVector& Y,FColor Color,float Radius,int32 NumSides, uint8 DepthPriority)
{
    const float	AngleDelta = 2.0f * PI / NumSides;
    FVector	LastVertex = Base + X * Radius;

    for(int32 SideIndex = 0; SideIndex < NumSides; SideIndex++)
    {
        const FVector Vertex = Base + (X * FMath::Cos(AngleDelta * (SideIndex + 1)) + Y * FMath::Sin(AngleDelta * (SideIndex + 1))) * Radius;
        new(lineBatchComponent->BatchedLines) FBatchedLine(LastVertex,Vertex,Color,100, thickness, DepthPriority);
        LastVertex = Vertex;
    }

    lineBatchComponent->MarkRenderStateDirty();
}

ACreatorBoard::ACreatorBoard()
{
    lineBatchComponent = CreateDefaultSubobject<ULineBatchComponent>(FName("Line Component"));
}

void ACreatorBoard::Tick(const float DeltaSeconds)
{
    lineBatchComponent->Flush();
    Super::Tick(DeltaSeconds);

    APlayerController* controller;

    float posX, posY;
    controller = GetWorld()->GetFirstPlayerController();
    controller->GetMousePosition(posX, posY);

    if (firstClick.IsSet())
    {
        if (!controller->IsInputKeyDown(EKeys::LeftMouseButton))
        {
            firstClick.Reset();
            return;
        }
    }
    else
    {
        if (controller->IsInputKeyDown(EKeys::LeftMouseButton))
        {
            UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(posX, posY);
            if (FVector worldDirection, worldPosition;
                    controller->DeprojectScreenPositionToWorld(posX, posY, worldPosition, worldDirection))
                        firstClick = worldPosition;
        }
        return;
    }
    FVector p1 = firstClick.GetValue();

    if (FVector worldDirection, worldPosition;
        controller->DeprojectScreenPositionToWorld(posX, posY, worldPosition, worldDirection))
    {
        worldPosition.Y = p1.Y;
        TArray<FVector> verts
        {
            p1,
            FVector(p1.X, p1.Y, worldPosition.Z),
            worldPosition,
            FVector(worldPosition.X, p1.Y, p1.Z),
        };
        for (auto& vert : verts)
        {
            vert.Y = GetActorLocation().Y;
            vert = GetActorTransform().InverseTransformPosition(vert);
            vert *= scale;
            vert = GetActorTransform().TransformPosition(vert);
        }
        
        const auto line1 = FBatchedLine(verts[0], verts[1], FColor::Red, 0, thickness, 0);
        const auto line2 = FBatchedLine(verts[1], verts[2], FColor::Red, 0, thickness, 0);
        const auto line3 = FBatchedLine(verts[2], verts[3], FColor::Red, 0, thickness, 0);
        const auto line4 = FBatchedLine(verts[3], verts[0], FColor::Red, 0, thickness, 0);

        TArray<FBatchedLine> lines { line1, line2, line3, line4 };
        lineBatchComponent->DrawLines(lines);
    }
}
UClass* ACreatorBoard::TileClass() const { return ACreatorTile::StaticClass(); }
