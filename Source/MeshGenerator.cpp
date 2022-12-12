// ReSharper disable CppParameterMayBeConst
// ReSharper disable CppLocalVariableMayBeConst
#include "MeshGenerator.h"

#include "Board.h"
#include "Vertex.h"
#include "Tile.h"
#include "ActiveSocket.h"
#include "BandagedModule.h"
#include "CreatorTile.h"
#include "CreatorBoard.h"
#include "TileColor.h"
#include "VectorTypes.h"
#include "CoordHeaders.h"
#include "KismetProceduralMeshLibrary.h"
#include "ModuleFactory.h"
#include "Kismet/KismetMathLibrary.h"

UMeshGenerator::UMeshGenerator() { }

#ifdef DRAW_DEBUG
void UMeshGenerator::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    for (int i = 0; i < vertices.Num(); i++)
        DrawDebugSphere(GetWorld(), vertices[i]->GetWorldPosition(), 1, 4, FColor::White);
}
#endif
bool LineLineIntersection(FVector startA, FVector endA, FVector startB, FVector endB, FVector& out_intersection)
{
    // Line AB represented as a1x + b1y = c1
    double a1 = endA.Z - startA.Z;
    double b1 = startA.X - endA.X;
    double c1 = a1 * (startA.X) + b1 * (startA.Z);
 
    // Line CD represented as a2x + b2y = c2
    double a2 = endB.Z - startB.Z;
    double b2 = startB.X - endB.X;
    double c2 = a2 * (startB.X) + b2 * (startB.Z);
 
    double determinant = a1 * b2 - a2 * b1;

    // Lines are parallel. Only occurs in squares, so we can just average the vertices
    if (FMath::Abs(determinant) < 0.01f)
    {
        out_intersection = (startA + startB) / 2;
        return true;
    }
    double x = (b2 * c1 - b1 * c2) / determinant;
    double z = (a1 * c2 - a2 * c1) / determinant;

    out_intersection = FVector(x, 0, z);
    return true;
}
ACreatorTile* GetCreatorTile(const Vertex* vertex) { return Cast<ACreatorTile>(vertex->GetTile()); }

// Recurisvely adds all tiles that are adjacent to each other
void GenerateBandageGroup(const TArray<ACreatorTile*>& tilesToMerge, TArray<ACreatorTile*>& mergeGroup, ACreatorTile* creatorTile)
{
    if (mergeGroup.Contains(creatorTile))
        return;
    mergeGroup.Add(creatorTile);

    for (const auto tile : creatorTile->GetAdjacent())
        if (tilesToMerge.Contains(tile))
            GenerateBandageGroup(tilesToMerge, mergeGroup, Cast<ACreatorTile>(tile));    
}

void UMeshGenerator::Merge()
{
    Unmerge();
    Merge(ACreatorTile::SelectedTiles());    
}
void UMeshGenerator::Merge(const TArray<ACreatorTile*>& tilesToMerge)
{
    // auto tilesToMerge = ACreatorTile::SelectedTiles();
    TArray<TArray<ACreatorTile*>> mergeGroups;

    for (auto creatorTile : tilesToMerge)
    {
        if (mergeGroups.Num() == 0)
        {
            mergeGroups.Add(TArray<ACreatorTile*>());
            GenerateBandageGroup(tilesToMerge, mergeGroups[0], creatorTile);
            goto nextTile;
        }
        for (auto mergeGroup : mergeGroups)
        {
            if (mergeGroup.Contains(creatorTile))
                goto nextTile;
        }
        
        // If we are here then the tile has not been included in any of the merge groups,
        // so create a new one and fill it up
        mergeGroups.Add(TArray<ACreatorTile*>());
        GenerateBandageGroup(tilesToMerge, mergeGroups.Last(), creatorTile);
        nextTile:;
    }
    for (int i = 0; i < mergeGroups.Num(); i++)
    {
        auto mergeGroup = mergeGroups[i];

        // Log("Merge size: ", mergeGroup.Num(), YELLOW);
        if (mergeGroup.Num() == 1)
            continue;
        TArray<Vertex*> queuedVertices;
        for (const auto& creatorTileA : mergeGroup)
            MergeWithNeighbors(mergeGroup, queuedVertices, creatorTileA);
        
        mergeGroup[0]->AddModule(ModuleFactory::Produce(EModule::Bandaged, mergeGroup[0]));
        auto bandagedModule = Cast<ABandagedModule>(mergeGroup[0]->CurrentSide()->GetModule(EModule::Bandaged));
        bandagedModule->Init();
        
        for (auto& vertex : queuedVertices)
            vertex->ApplyPosition();
        for (const auto& creatorTile : mergeGroup)
        {
            creatorTile->MeshGenerator->Draw(false);
            bandagedModule->AddModTile(creatorTile);
        }
        bandagedModule->Finish();
        mergeGroup[0]->SetColor(GetBandagedColor(mergeGroup), true);
    }
}
void UMeshGenerator::Unmerge() { Unmerge(ACreatorTile::SelectedTiles()); }
void UMeshGenerator::Unmerge(TArray<ACreatorTile*> tilesToUnmerge)
{
    if (tilesToUnmerge.Num() == 0)
        return;
    TArray<ABandagedModule*> modules;

    for (const auto creatorTile : tilesToUnmerge)
    {
        {
            // TODO:: if this gets merged afterwards then this draw operation is wasted
            //      : its only here to reset vertices, but if it could know that its going to merge
            //      : it could just call the reset vertices method and save some time
        }
        creatorTile->MeshGenerator->Draw(true);

        if (auto bandagedModule = Cast<ABandagedModule>(creatorTile->CurrentSide()->GetModule(EModule::Bandaged)))
        {
            // Back up tiles to remove to prevent mutating the array during the for loop
            TArray<AModTile*> bandageTiles = bandagedModule->Tiles;
            for (auto tile : bandageTiles)
                if (tilesToUnmerge.Contains(tile))
                    bandagedModule->RemoveModTile(tile);
            bandagedModule->RemoveModTile(creatorTile);
            modules.AddUnique(bandagedModule);
        }
    }
    
    TArray<TArray<ACreatorTile*>> remainingTiles;
    int i = 0;
    for (auto module : modules)
    if (module)
    {
        remainingTiles.Add(TArray<ACreatorTile*>());
        for (auto tile : module->Tiles)
            remainingTiles[i].AddUnique(static_cast<ACreatorTile*>(tile));
        for (auto tile : remainingTiles[i])
            module->RemoveModTile(tile);
        i++;
    }
    for (auto mergeGroup : remainingTiles)
    {
        Unmerge(mergeGroup);
        Merge(mergeGroup);
    }
}

void UMeshGenerator::MergeWithNeighbors(const TArray<ACreatorTile*>& tilesToMerge, TArray<Vertex*>& queuedVertices, ACreatorTile* const& creatorTileA)
{
    for (auto vertexA : creatorTileA->MeshGenerator->vertices)
    {
        TArray<ACreatorTile*> selectedNeighbors;
        for (auto vertexB : vertexA->Neighbors())
        {
            const auto creatorTileB = GetCreatorTile(vertexB);
            if (tilesToMerge.Contains(creatorTileB))
                selectedNeighbors.Add(creatorTileB);
        }
        if (selectedNeighbors.Num() == 1)
            MergeTwoVertices(tilesToMerge, queuedVertices, creatorTileA, vertexA);
        if (selectedNeighbors.Num() == 2 && creatorTileA->Board()->GetBoardShape() == EBoardShape::Triangle)
            MergeThreeVertices(queuedVertices, creatorTileA, vertexA, selectedNeighbors);
        else if (selectedNeighbors.Num() > 1)
            MergeMultipleVertices(tilesToMerge, vertexA);
    }
}
bool UMeshGenerator::MergeWithNeighbor(TArray<Vertex*>& queuedVertices, ACreatorTile* const& creatorTileA, Vertex* vertexA, ACreatorTile* const& creatorTileB)
{
    FVector intersection;

    for (auto& vertexB : creatorTileB->MeshGenerator->vertices)
    if (ShouldMergeVertices(vertexA, vertexB) &&
        IsIntersectionValid(Cast<ACreatorBoard>(creatorTileA->Board())->VertexMode, creatorTileA, creatorTileB, vertexA, vertexB, intersection))
    {
        QueueVertices(queuedVertices, creatorTileA, vertexA, vertexB, intersection);
        return true;
    }
    return false;
}

void UMeshGenerator::MergeTwoVertices(const TArray<ACreatorTile*>& tilesToMerge, TArray<Vertex*>& queuedVertices, ACreatorTile* const& creatorTileA, Vertex* vertexA)
{
    if (!vertexA->IsMerged())
        for (const auto& creatorTileB : tilesToMerge)
            MergeWithNeighbor(queuedVertices, creatorTileA, vertexA, creatorTileB);
}
void UMeshGenerator::MergeThreeVertices(TArray<Vertex*>& queuedVertices, ACreatorTile* const& creatorTileA, Vertex* vertexA, TArray<ACreatorTile*> selectedNeighbors)
{
    if (vertexA->IsMerged())
        return;
    ACreatorTile* middleTile = nullptr;
    TOptional<FVector> intersection;

    for (const auto& creatorTileB : selectedNeighbors)
    {
        const FTriCoord* triCoordA = static_cast<const FTriCoord*>(creatorTileA->GetCoord().Get());
        const FTriCoord* triCoordB = static_cast<const FTriCoord*>(creatorTileB->GetCoord().Get());

        // TriTiles that face the same direction have their vertices aligned for an intersection
        if (triCoordA->GetIsUp() == triCoordB->GetIsUp())
        {
            // Cache the intersection to use for the middle TriTile that doesn't intersect,
            // but needs the same vertex position
            if (MergeWithNeighbor(queuedVertices, creatorTileA, vertexA, creatorTileB))
                intersection = TOptional<FVector>(vertexA->GetQueuedPosition());
        }
        else middleTile = creatorTileB;
    }

    if (!intersection.IsSet())
        return;
    
    // This snaps the odd TriTiles vertex to the same intersection point gained from the even TriTiles
    for (auto& vertexB : middleTile->MeshGenerator->vertices)
    if (ShouldMergeVertices(vertexA, vertexB))
    {
        vertexB->QueuedPosition(*intersection);
        queuedVertices.Add(vertexB);
    }
}
void UMeshGenerator::MergeMultipleVertices(const TArray<ACreatorTile*>& tilesToMerge, const Vertex* vertexA)
{
    FVector sum = FVector::Zero();
    TArray<Vertex*> selectedVertices;

    for (auto vertexB : vertexA->Neighbors())
        if (tilesToMerge.Contains(GetCreatorTile(vertexB)))
        {
            selectedVertices.Add(vertexB);
            sum += vertexB->GetWorldPosition();
        }
    AverageVertices(selectedVertices, sum);
}

bool UMeshGenerator::ShouldMergeVertices(const Vertex* vertexA, const Vertex* vertexB)
{
    // TODO:: add this check back if board is not triangle
    // if (vertexA->IsMerged() || vertexB->IsMerged())
        // return false;
    return vertexA->Neighbors().Contains(vertexB);
}
void UMeshGenerator::QueueVertices(TArray<Vertex*>& queuedVertices, ACreatorTile* const& creatorTileA, Vertex* vertexA, Vertex* vertexB, FVector intersection)
{
#ifdef DRAW_DEBUG
    DrawDebugSphere(creatorTileA->GetWorld(), intersection, 5, 16, FColor::Green, true);
#endif
    vertexA->QueuedPosition(intersection);
    vertexB->QueuedPosition(intersection);
    queuedVertices.Add(vertexA);
    queuedVertices.Add(vertexB);
}
bool UMeshGenerator::IsIntersectionValid(EVertexMode vertexMode, const ACreatorTile* const creatorTileA, const ACreatorTile* const creatorTileB, const Vertex* vertexA, const Vertex* vertexB, FVector& intersection)
{
    // There are two vertexes, prev & next, that can be used as the starting FVector for the intersection line
    // The vertex thats closest to the tile is also being merged, so we want to avoid it
    // This function seeks the vertex that is furtherst away from the tile
    const double location1 = FVector::Distance(creatorTileB->GetActorLocation(), vertexA->NextVertex()->GetWorldPosition());
    const double location2 = FVector::Distance(creatorTileB->GetActorLocation(), vertexA->PrevVertex()->GetWorldPosition());
    const double location3 = FVector::Distance(creatorTileA->GetActorLocation(), vertexB->NextVertex()->GetWorldPosition());
    const double location4 = FVector::Distance(creatorTileA->GetActorLocation(), vertexB->PrevVertex()->GetWorldPosition());
    
    Vertex* vA = location1 >= location2 ? vertexA->NextVertex() : vertexA->PrevVertex();
    Vertex* vB = location3 >= location4 ? vertexB->NextVertex() : vertexB->PrevVertex();

    return GetIntersection(creatorTileA->GetWorld(), vA, vertexA, vB, vertexB, intersection);
}
bool UMeshGenerator::GetIntersection(const UWorld* worldContext, const Vertex* startA, const Vertex* endA, const Vertex* startB, const Vertex* endB, FVector& intersection)
{
    const FVector lineAStart = startA->GetWorldPosition(), lineAEnd = GetEndVertex(startA, endA);
    const FVector lineBStart = startB->GetWorldPosition(), lineBEnd = GetEndVertex(startB, endB);

#ifdef DRAW_DEBUG
    DrawDebugLine(worldContext, lineAStart, lineAEnd, FColor::Green, true, 100, 100);
    DrawDebugLine(worldContext, lineBStart, lineBEnd, FColor::White, true, 100, 100);
#endif
    bool result = LineLineIntersection(lineAStart, lineAEnd, lineBStart, lineBEnd, intersection);

    if (intersection.Length() > 1000)
        return false;
    return result;
}

void UMeshGenerator::AverageVertices(TArray<Vertex*> neighbors, FVector sum)
{
    for (const auto& neighbor : neighbors)
        neighbor->SetPosition(sum / neighbors.Num());
}
FVector UMeshGenerator::GetEndVertex(const Vertex* start, const Vertex* end)
{
    // This function isn't technically needed, as the intersection uses lines, not line segments,
    // but it makes the visualization easier
    const FVector startPos = start->GetWorldPosition();
    const FTransform startTrans{ startPos };
    FVector transformed = startTrans.InverseTransformPosition(end->GetWorldPosition());
    transformed.Normalize();

    return end->GetWorldPosition() + transformed * 20;
}
ETileColor UMeshGenerator::GetBandagedColor(const TArray<ACreatorTile*>& tilesToMerge)
{
    TMap<ETileColor, int> colorMap;
    ETileColor bandagedColor = ETileColor::None;
    int maxColor = 0;

    for (int i = 0; i < tilesToMerge.Num(); i++)
    {
        const auto& currentColor = tilesToMerge[i]->GetColor();
        const auto TrySetBandageColor = [&]()
        {
            if (colorMap[currentColor] > maxColor)
            {
                maxColor = colorMap[currentColor];
                bandagedColor = currentColor;
            }
        };

        if (colorMap.Contains(currentColor))
        {
            colorMap[currentColor]++;

            if (colorMap[currentColor] > tilesToMerge.Num() / 2)      // If the majority of the tiles to merge are one color, exit early
            {
                bandagedColor = currentColor;
                break;
            }
            TrySetBandageColor();
        }
        else
        {
            colorMap.Add(currentColor, 1);
            TrySetBandageColor();
        }
    }
    return bandagedColor;
}

void UMeshGenerator::Init(int _radius, int _vertexCount, int _angleOffset, int _angle)
{
    radius = _radius, vertexCount = _vertexCount, angleOffset = _angleOffset, angle = _angle;
    Draw(true);
}
void UMeshGenerator::LinkVertices()
{
    for (const auto vertex : vertices)
        vertex->LinkVertices();
}
void UMeshGenerator::UpdateMesh()
{
    TArray<int> triangles;
    TArray<FVector> normals;
    TArray<FVector2D> UV;
    TArray<FColor> colors;
    TArray<FProcMeshTangent> tangents;
    
    int triangleCount = roundedVertices.Num();
    UKismetProceduralMeshLibrary::CreateGridMeshTriangles(triangleCount, triangleCount, false, triangles);
    
    for (int i = 0; i < roundedVertices.Num(); i++)
        normals.Add(FVector::RightVector);
    for (int i = 0; i < roundedVertices.Num(); i++)
    {
        auto vert = roundedVertices[i];
        auto vert2D = FVector2D(vert.X, vert.Z);

        UV.Add(vert2D.GetSafeNormal());
    }
    ProceduralMesh->CreateMeshSection(0, roundedVertices, triangles, normals, UV, colors, tangents, true);
}
void UMeshGenerator::ResetVertices()
{
    for (auto vertex : vertices)
        vertex->Unmerge();
}
void UMeshGenerator::Draw(bool resetVertices)
{
    if (resetVertices)
    {
        if (vertices.Num() == 0)
        {
            for (int i = 0; i < vertexCount; i++)
                vertices.Add(new Vertex(i, vertexCount, FVector(radius * UKismetMathLibrary::DegCos(i * angle + angleOffset), 0
                                                              , radius * UKismetMathLibrary::DegSin(i * angle + angleOffset)), this));
            // Vertices need a delay inorder to link up their neighbors correctly
            FTimerHandle handle;
            GetOwner()->GetWorldTimerManager().SetTimer(handle, [&]() { LinkVertices(); }, .01f, false);
        }
        else ResetVertices();
    }
    const float circleRadius = 15;

    auto shape = Cast<ATile>(GetOwner())->Board()->GetBoardShape();
    float distance = shape == EBoardShape::Square   ? FMath::Sqrt(2.0f)
                   : shape == EBoardShape::Triangle ? 1.75f : 1.15f;
    TArray<FVector> circleOrigins;
    for (const auto& vertex : vertices)
        circleOrigins.Add(-vertex->GetLocalPosition().GetSafeNormal() * circleRadius * distance + vertex->GetLocalPosition());

    roundedVertices.Empty();
    for (int vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++)
    {
        if (!vertices[vertexIndex]->ShouldRound())
        {
            roundedVertices.Add(vertices[vertexIndex]->GetLocalPosition());
            continue;
        }
        FVector circleOrigin = circleOrigins[vertexIndex];
    
        int curveCount = 5;
        for (int curveIndex = -curveCount; curveIndex <= curveCount; curveIndex++)
        {
            FTransform circleTrans{ circleOrigin };
            FVector edge = circleOrigin.GetSafeNormal() * circleRadius + circleOrigin;
    
            edge = circleTrans.InverseTransformPosition(edge);
            edge = FRotator(curveIndex * 30 / curveCount, 0, 0).RotateVector(edge);
            edge = circleTrans.TransformPosition(edge);
    
            roundedVertices.Add(edge);
        }
    }
    UpdateMesh();
}//368