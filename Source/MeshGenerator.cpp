#include "MeshGenerator.h"
#include "Board.h"
#include "Vertex.h"
#include "Tile.h"
#include "BandagedModule.h"
#include "CreatorTile.h"
#include "TileColor.h"
#include "VectorTypes.h"
#include "CoordHeaders.h"
#include "ModuleFactory.h"
#include "TileSide.h"
#include "TimerManager.h"
#include "KismetProceduralMeshLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#ifdef DRAW_DEBUG
#include "DrawDebugHelpers.h"
#endif

UMeshGenerator::UMeshGenerator() { }

#ifdef DRAW_DEBUG
void UMeshGenerator::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    for (auto [index, vertex] : vertices) 
        DrawDebugSphere(GetWorld(), vertex->GetWorldPosition(), 1 + index * 1, 4, FColor::White);
}
#endif
bool LineLineIntersection(FVector startA, FVector endA, FVector startB, FVector endB, FVector& out_intersection)
{
    bool _;
    return LineLineIntersection(startA, endA, startB, endB, out_intersection, _);
}
bool LineLineIntersection(FVector startA, FVector endA, FVector startB, FVector endB, FVector& out_intersection, bool& isParallel)
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
        isParallel = true;
        return true;
    }
    double x = (b2 * c1 - b1 * c2) / determinant;
    double z = (a1 * c2 - a2 * c1) / determinant;

    out_intersection = FVector(x, 0, z);
    return true;
}
ACreatorTile* GetCreatorTile(const Vertex* vertex) { return Cast<ACreatorTile>(vertex->GetTile()); }

// Recurisvely generates groups of tiles, based on adjacency
void GenerateBandageGroup(const TArray<ACreatorTile*>& tilesToMerge, TArray<ACreatorTile*>& mergeGroup, ACreatorTile* creatorTile)
{
    if (mergeGroup.Contains(creatorTile))
        return;
    mergeGroup.Add(creatorTile);

    for (const auto tile : creatorTile->GetNeighbors())
        if (tilesToMerge.Contains(tile))
            GenerateBandageGroup(tilesToMerge, mergeGroup, Cast<ACreatorTile>(tile));
}
void UMeshGenerator::Merge(bool selectTiles)
{
    TArray<ACreatorTile*> tilesToMerge = ACreatorTile::SelectedTiles();
    
    if (selectTiles)
    {
        for (auto selectedTile : ACreatorTile::SelectedTiles())
        {
            if (!selectedTile->HasModule(EModule::Bandaged))
                continue;
            auto bandagedModule = Cast<ABandagedModule>(selectedTile->GetModule(EModule::Bandaged));

            for (auto tile : bandagedModule->Tiles)
            {
                tilesToMerge.Add(Cast<ACreatorTile>(tile));
                tilesToMerge.Last()->Select(true, true);
            }
        }
    }
    // Unmerges any tiles that were in a previous bandage
    Unmerge(false);
    Merge(tilesToMerge);
}
void UMeshGenerator::Merge(const TArray<ACreatorTile*>& tilesToMerge)
{
    TArray<TArray<ACreatorTile*>> mergeGroups;

    // Get Merge Groups
    for (auto creatorTile : tilesToMerge)
    {
        if (mergeGroups.Num() == 0)
        {
            mergeGroups.Add(TArray<ACreatorTile*>());
            GenerateBandageGroup(tilesToMerge, mergeGroups[0], creatorTile);
            goto nextTile;
        }
        for (auto mergeGroup : mergeGroups)
            if (mergeGroup.Contains(creatorTile))
                goto nextTile;

        // If we are here then the tile has not been included in any of the merge groups,
        // so create a new one and fill it up
        mergeGroups.Add(TArray<ACreatorTile*>());
        GenerateBandageGroup(tilesToMerge, mergeGroups.Last(), creatorTile);
    nextTile:;
    }
    // Merge all tiles in each group
    for (int i = 0; i < mergeGroups.Num(); i++)
    {
        auto mergeGroup = mergeGroups[i];

        if (mergeGroup.Num() == 1)
            continue;
        TArray<Vertex*> queuedVertices;
        for (const auto& creatorTileA : mergeGroup)
            MergeWithNeighbors(mergeGroup, queuedVertices, creatorTileA);

        mergeGroup[0]->AddModule(ModuleFactory::Produce(EModule::Bandaged, mergeGroup[0]));
        auto bandagedModule = Cast<ABandagedModule>(mergeGroup[0]->GetModule(EModule::Bandaged));
        bandagedModule->Init();

        // Position was queued to avoid affecting the math of other vertices. 
        for (auto& vertex : queuedVertices)
            vertex->ApplyPosition();
        for (const auto& creatorTile : mergeGroup)
        {
            creatorTile->MeshGenerator->Draw(false, tilesToMerge);
            bandagedModule->AddModTile(creatorTile);
        }
        bandagedModule->Finish();
        mergeGroup[0]->SetColor(GetBandagedColor(mergeGroup), true);
    }
}
void UMeshGenerator::Unmerge(bool shouldRedraw) { Unmerge(ACreatorTile::SelectedTiles(), shouldRedraw); }
void UMeshGenerator::Unmerge(TArray<ACreatorTile*> tilesToUnmerge, bool shouldRedraw)
{
    if (tilesToUnmerge.Num() == 0)
        return;
    TArray<ABandagedModule*> modules;

    // if (!shouldRedraw)
    for (const auto creatorTile : tilesToUnmerge)
            creatorTile->MeshGenerator->ResetVertices();
    for (const auto creatorTile : tilesToUnmerge)
    {
        if (shouldRedraw)
             creatorTile->MeshGenerator->Draw(false, tilesToUnmerge);
        else creatorTile->MeshGenerator->ResetVertices();

        if (auto bandagedModule = Cast<ABandagedModule>(creatorTile->CurrentSide()->GetModule(EModule::Bandaged)))
        {
            // Back up tiles to remove to prevent mutating the array during the for loop
            TArray<AModTile*> bandageTiles = bandagedModule->Tiles;
            for (auto tile : bandageTiles)
                if (tilesToUnmerge.Contains(tile)) bandagedModule->RemoveModTile(tile);
            bandagedModule->RemoveModTile(creatorTile);
            modules.AddUnique(bandagedModule);
        }
    }

    // If merge selection cuts through one or more bandagedTile, this maintains those merge groups
    // So we can merge them back, but without the tiles that have been unmerged
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

    // Update any tiles that remain that should be merged with tiles from a previous merge group
    for (auto mergeGroup : remainingTiles)
    {
        Unmerge(mergeGroup, true); // Refresh the status of the tiles before merging
        Merge(mergeGroup);
    }
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
    for (auto& [_, vertexB] : middleTile->MeshGenerator->vertices)
        if (ShouldMergeVertices(vertexA, vertexB))
        {
            vertexB->QueuePosition(*intersection, 3);
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

bool UMeshGenerator::MergeWithNeighbor(TArray<Vertex*>& queuedVertices, ACreatorTile* const& creatorTileA, Vertex* vertexA, ACreatorTile* const& creatorTileB)
{
    FVector intersection;

    for (auto& [_, vertexB] : creatorTileB->MeshGenerator->vertices)
        if (ShouldMergeVertices(vertexA, vertexB)
            && IsIntersectionValid(creatorTileA, creatorTileB, vertexA, vertexB, intersection))
        {
            QueueVertices(queuedVertices, creatorTileA, vertexA, vertexB, intersection);
            return true;
        }
    return false;
}
void UMeshGenerator::MergeWithNeighbors(const TArray<ACreatorTile*>& tilesToMerge, TArray<Vertex*>& queuedVertices, ACreatorTile* const& creatorTileA)
{
    for (auto [_, vertexA] : creatorTileA->MeshGenerator->vertices)
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

void UMeshGenerator::QueueVertices(TArray<Vertex*>& queuedVertices, ACreatorTile* const& creatorTileA, Vertex* vertexA, Vertex* vertexB, FVector intersection)
{
#ifdef DRAW_DEBUG
    DrawDebugSphere(creatorTileA->GetWorld(), intersection, 5, 16, FColor::Green, true);
#endif
    vertexA->QueuePosition(intersection);
    vertexB->QueuePosition(intersection);
    queuedVertices.Add(vertexA);
    queuedVertices.Add(vertexB);
}
bool UMeshGenerator::ShouldMergeVertices(const Vertex* vertexA, const Vertex* vertexB) { return vertexA->Neighbors().Contains(vertexB); }
bool UMeshGenerator::IsIntersectionValid(const ACreatorTile* const creatorTileA, const ACreatorTile* const creatorTileB, const Vertex* vertexA, const Vertex* vertexB, FVector& intersection)
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
    return intersection.Length() > 1000 ? false : result;
}
void UMeshGenerator::AverageVertices(TArray<Vertex*> neighbors, FVector sum)
{
    for (const auto& neighbor : neighbors)
        neighbor->SetPosition(sum / neighbors.Num(), neighbors.Num());
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

            if (colorMap[currentColor] > tilesToMerge.Num() / 2) // If the majority of the tiles to merge are one color, exit early
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

ATile* UMeshGenerator::GetTile() const      { return Cast<ATile>(GetOwner()); }
void UMeshGenerator::ResetVertices()        { for (auto [_, vertex] : vertices) vertex->Unmerge(); }
void UMeshGenerator::SetupVertexNeighbors() { for (auto [_, vertex] : vertices) vertex->SetupNeighbors(); }
void UMeshGenerator::UpdateMesh()
{
    TArray<int> triangles;
    TArray<FVector> normals;
    TArray<FVector2D> UV;
    TArray<FColor> colors;
    TArray<FProcMeshTangent> tangents;

    int triangleCount = roundedVertices.Num();
    UKismetProceduralMeshLibrary::CreateGridMeshTriangles(triangleCount, triangleCount, false, triangles);

    for (int i = 0; i < roundedVertices.Num(); i++) normals.Add(FVector::RightVector);
    for (int i = 0; i < roundedVertices.Num(); i++)
    {
        auto vert = roundedVertices[i];
        auto vert2D = FVector2D(vert.X, vert.Z);

        UV.Add(vert2D.GetSafeNormal());
    }
    ProceduralMesh->CreateMeshSection(0, roundedVertices, triangles, normals, UV, colors, tangents, true);
}
void UMeshGenerator::Draw(bool resetVertices, TArray<ACreatorTile*> tilesToMerge)
{
    EBoardShape shape = Cast<ATile>(GetOwner())->Board()->GetBoardShape();

    int indexOffset = 0;
    
    // ReSharper disable once CppTooWideScope
    bool isUpsideDownTriangle = shape == EBoardShape::Triangle && !static_cast<const FTriCoord*>(Cast<ATile>(GetOwner())->GetCoord().Get())->GetIsUp();
    if (isUpsideDownTriangle)
        indexOffset = 1;
    
    if (resetVertices)
    {
        if (vertices.Num() != 0)
            ResetVertices();
        else
        {
            // Since triangles can be up or down, this system ensures that vertex indexes will be different, even if they map to the same place
            // Upside down triangles use vertex indexes 1, 3, and 5, while upright triangles use indexes 0, 2, 4
            int maxIndex = vertexCount * (1 + (shape == EBoardShape::Triangle ? 1 : 0));
            int indexIncrement = 1 + (shape == EBoardShape::Triangle ? 1 : 0);
            
            for (int j = 0, i = indexOffset; i < maxIndex; i += indexIncrement, j++)
            {
                float x = radius * UKismetMathLibrary::DegCos(j * angle + angleOffset);
                float z = radius * UKismetMathLibrary::DegSin(j * angle + angleOffset);
                vertices.Add(i, new Vertex(i, vertexCount, FVector(x, 0, z), this, maxIndex, indexIncrement));
            }
            FTimerHandle handle; // Vertices need a delay inorder to link up their neighbors correctly
            GetOwner()->GetWorldTimerManager().SetTimer(handle, [&] { SetupVertexNeighbors(); }, .01f, false);
        }
    }
    const float circleRadius = 15;

    float distance = shape == EBoardShape::Square
                         ? FMath::Sqrt(2.0f) : shape == EBoardShape::Triangle
                         ? 2.f : 1.15f;
    TMap<int, FVector> circleOrigins;
    for (const auto& [index, vertex]: vertices)
        circleOrigins.Add(index, -vertex->GetLocalPosition().GetSafeNormal() * circleRadius * distance + vertex->GetLocalPosition());

    roundedVertices.Empty();
    TArray<int> indexArray;
    vertices.GenerateKeyArray(indexArray);
    for (auto i : indexArray) 
    {
        auto vertexIndex = i;
        Vertex* curVertex = vertices[vertexIndex];
        const int vertexSpacing = GetTile()->Shape() == EBoardShape::Triangle ? 60 : 30;

        // Only round if neighbor vertices that are being merged also need to round
        bool shouldRound = curVertex->ShouldRound();
        if (shouldRound)
            for (auto neighbor : curVertex->Neighbors())
                if (tilesToMerge.Contains(Cast<ACreatorTile>(neighbor->GetTile())))
                    shouldRound = shouldRound && neighbor->ShouldRound();
        if (!shouldRound)
        {
            roundedVertices.Add(curVertex->GetLocalPosition());
            continue;
        }
        FVector circleOrigin = circleOrigins[vertexIndex];

        // Round corners that aren't merged
        if (curVertex->Shape() == EBoardShape::Triangle && !curVertex->IsMerged() || curVertex->Shape() != EBoardShape::Triangle)
        {
            int curveCount = 5;
            for (int curveIndex = -curveCount; curveIndex <= curveCount; curveIndex++)
            {
                FTransform circleTrans{ circleOrigin };
                FVector edge = circleOrigin.GetSafeNormal() * circleRadius + circleOrigin;
            
                edge = circleTrans.InverseTransformPosition(edge);
                edge = FRotator(curveIndex * vertexSpacing / curveCount, 0, 0).RotateVector(edge);
                edge = circleTrans.TransformPosition(edge);
                roundedVertices.Add(edge);
            }
        }
        // Round corners that have been merged. Only possible in Triangle Boards, when two vertices merge
        else
        {
            const int curveCount = 6;
            auto ogRot = GetTile()->GetActorRotation();
            GetTile()->SetActorRotation(FRotator::ZeroRotator);
            
            for (int curveIndex = -curveCount; curveIndex <= curveCount; curveIndex++)
            {
                float extraRadius = 1.45f;
                Vertex* opposite  = vertices[vertexIndex];

                //  BUG:: If there is a tie between most neighbors, it can cause deformation if it picks the wrong tile
                //      : Occurs in convex turns
                int mostMerges = 0;
                for (auto [_, vertex] : vertices)
                {
                    if (vertex != curVertex && vertex->GetMergeCount() > mostMerges)
                    {
                        opposite   = vertex;
                        mostMerges = vertex->GetMergeCount();
                    }
                }
                auto midpoint = (opposite->GetWorldPosition() + curVertex->GetWorldPosition()) / 2;
                auto v2 = FTransform(midpoint).InverseTransformPosition(curVertex->GetWorldPosition());

#ifdef DRAW_DEBUG
                if (GetTile()->ID() == 7 || GetTile()->ID() == 2)
                {
                    DrawDebugSphere(GetWorld(), opposite->GetWorldPosition(), 7, 8, FColor::Red, true);
                    DrawDebugSphere(GetWorld(), midpoint, 7, 8, FColor::Black, true);
                    DrawDebugSphere(GetWorld(), curVertex->GetWorldPosition(), 7, 8, FColor::Blue, true);
                }
#endif

                circleOrigin = FTransform(curVertex->GetTile()->GetActorLocation())
                        .InverseTransformPosition(curVertex->GetWorldPosition() - v2.GetSafeNormal() * distance * circleRadius * extraRadius / 1.75f);
                FTransform circleTrans{ circleOrigin };
                FTransform vertexTrans{ curVertex->GetWorldPosition() };
                vertexTrans.SetLocation(FVector(vertexTrans.GetLocation().X, 0, vertexTrans.GetLocation().Z));
                FVector edge = circleOrigin.GetSafeNormal() * circleRadius * extraRadius;

#ifdef DRAW_DEBUG
                // if (GetTile()->ID() == 7 || GetTile()->ID() == 2)
                // {
                //     DrawDebugSphere(GetWorld(), midpoint, 7, 8, FColor::Black, true);
                //     DrawDebugSphere(GetWorld(), GetTile()->GetTransform().TransformPosition(circleOrigin), 7, 8, FColor::Blue,  true);
                // }
#endif
                
                edge = FRotator(curveIndex * vertexSpacing * 1.45f / curveCount, 0, 0).RotateVector(edge);
                edge = circleTrans.TransformPosition(edge);
                roundedVertices.Add(edge);
            }
            GetTile()->SetActorRotation(ogRot);
        }

        i++;
    }
    UpdateMesh();
} //368, 480