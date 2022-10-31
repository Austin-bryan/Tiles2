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

    float posX, posY;
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(posX, posY);
    FVector position(posX, 0, posY);
    //
    APlayerController* controller = GetWorld()->GetFirstPlayerController();

    FVector p1(-122, 780, 9185);

    if (FVector worldDirection, worldPosition;
        controller->DeprojectScreenPositionToWorld(posX, posY, worldPosition, worldDirection))
    {
        // worldPosition.Y = GetActorLocation().Y;
        // worldPosition = worldPosition - FVector::UnitX() * 100 + FVector::UnitZ() * 100;
        //const auto vert1 = worldPosition - FVector::UnitX() * 100 + FVector::UnitZ() * 100;
        const auto vert1 = p1;
        // const auto vert2 = FVector(p1.X, p1.Y, position.Z);
        const auto vert2 = FVector(p1.X, p1.Y, worldPosition.Z);
        // const auto vert3 = position;
        const auto vert3 = worldPosition;
        //const auto vert4 = FVector(position.X, p1.Y, p1.Z);
        const auto vert4 = FVector(worldPosition.X, p1.Y, p1.Z);
        // const auto vert2 = GetActorLocation() + FVector::UnitY() * 50 + FVector::UnitX() * 100 + FVector::UnitZ() * 100;
        // const auto vert3 = GetActorLocation() + FVector::UnitY() * 50 + FVector::UnitX() * 100 - FVector::UnitZ() * 100;
        // const auto vert4 = GetActorLocation() + FVector::UnitY() * 50 - FVector::UnitX() * 100 - FVector::UnitZ() * 100;
        
        const auto line1 = FBatchedLine(vert1, vert2, FColor::Black, 0, thickness, 0);
        const auto line2 = FBatchedLine(vert2, vert3, FColor::Black, 0, thickness, 0);
        const auto line3 = FBatchedLine(vert3, vert4, FColor::Black, 0, thickness, 0);
        const auto line4 = FBatchedLine(vert4, vert1, FColor::Black, 0, thickness, 0);
    // UGameplayStatics::GetPlayerController(GetWorld(), 0)->
        // DeprojectScreenPositionToWorld(position.X, position.Y, worldPosition, worldDirection);

        TArray<FBatchedLine> lines { line1, line2, line3, line4 };
        lineBatchComponent->DrawLines(lines);
    }
    
    
    DrawCircle(GetActorLocation() + FVector::UnitY() * 50,
        FVector::UnitZ(), FVector::UnitX(), FColor::Green, 100, 64, 0);
}
UClass* ACreatorBoard::TileClass() const { return ACreatorTile::StaticClass(); }
