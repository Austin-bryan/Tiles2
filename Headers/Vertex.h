#pragma once
#include "CoreMinimal.h"
#include "MeshGenerator.h"

class UMeshGenerator;

class TILES2_API Vertex
{
    friend class AStaticResetter;

    // TODO:: remove this:
    friend class AntMarchSpawner; 
public:
    Vertex(int index, int sideCount, FVector position, UMeshGenerator* generator, int maxIndex, int indexIncrement);
    
    ATile*  GetTile() const;
    bool    IsMerged() const;
    bool    ShouldRound() const;
    Vertex* NextVertex() const;
    Vertex* PrevVertex() const;
    FVector GetLocalPosition() const;
    FVector GetWorldPosition() const;
    FVector GetQueuedPosition() const;
    bool    IsOnBoardEdge() const;
    void    Unmerge();
    const   TArray<Vertex*>& Neighbors() const { return neighbors; }

    float X() const { return GetWorldPosition().X; }
    float Y() const { return GetWorldPosition().Y; }
    float Z() const { return GetWorldPosition().Z; }

    void SetupNeighbors();
    void ApplyPosition();
    void QueuePosition(const FVector newPosition, int _mergeCount = 2);
    void SetPosition(const FVector newPosition, int _mergeCount = -1);
    void SetShouldRound(bool value);
    EBoardShape Shape() const { return GetTile()->Shape(); }
    int GetMergeCount() const { return mergeCount; }

    static bool AreNeighbors(const Vertex* a, const Vertex* b);
    static bool AreCollinear(const Vertex* a, const Vertex* b, const Vertex* c);
    inline static float Distance(const Vertex* a, const Vertex* b);
    bool operator==(const Vertex* rhs) const;
    bool operator==(const Vertex rhs) const;

    // TODO:: Make private again
// private:
    static TArray<Vertex*> Vertices;

    int ID, index, sideCount, maxIndex, indexIncrement, mergeCount = 0;
    bool hasBeenMerged = false, shouldRoundWhileMerged = false;
    
    TArray<Vertex*> neighbors;
    UMeshGenerator* generator;
    FVector position, queuedPosition;
    FVector unmergedPosition;
};