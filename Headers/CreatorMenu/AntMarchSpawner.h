#pragma once
#include "CoreMinimal.h"
#include "AntMarchLineBase.h"
#include "Coord.h"
#include "CopySignature.h"
#include "CreatorTile.h"

//#define SHOW_DEBUG

class Vertex;
class ACreatorBoard;
class ASeedCreator;
using TraversedVertices = TTuple<TArray<Vertex*>, TArray<TArray<Vertex*>>>;

class AntMarchSpawner 
{
    class VertexBookkeeper;
public:
    explicit AntMarchSpawner(ACreatorBoard* creatorBoard) : creatorBoard{ creatorBoard } {  }
    void Draw(const TMap<FCoord, CopySignature>& _copiedMap);
    void HideLines();
    void BindToSeedChange(ASeedCreator* seedCreator);
private:
    VertexBookkeeper GetOuterVertices(int size) const;
    void TraverseVertices(VertexBookkeeper& bookkeeper, int size) const;

    // TODO:: Is this needed?
    TArray<TArray<FVector>> CenterAntVertices(VertexBookkeeper& bookkeeper) const;

    TArray<AAntMarchLineBase*> lines;
    TMap<FCoord, CopySignature> copiedMap;
    ACreatorBoard* creatorBoard;
    
    static Vertex* GetVertex(const TArray<Vertex*>& vs, int index);

    // TODO:: Is this needed?
    static int Bounds;
    
    class VertexTraveler
    {
    public:
        static TUniquePtr<VertexTraveler> Create(const VertexBookkeeper& bookkeeper, int size);
        explicit VertexTraveler(Vertex* minVertex, bool isOuter, int size, int indexDistance);
        virtual ~VertexTraveler() = default;
        
        void NextExternalVertex(int direction);
        void NextInternalVertex(int direction);
        void TraverseVertexRing(VertexBookkeeper& bookkeeper);
        bool HasChangedTiles(VertexBookkeeper& bookkeeper);
        void ProcessVertex(VertexBookkeeper& bookkeeper, const Vertex* vertex, int vertexIndex) const;
    protected:
        virtual void Travel(VertexBookkeeper& bookkeeper, Vertex*& travelingVertex, int& travelingIndex);
        Vertex* GetNeighborFromVertex(const ATile* tile, int vertexIndex, int neighborIndex) const;

        int exitVertexIndex = -1, externalVertexIndex = 0, internalVertexIndex = 0, startIndex = 0, prevInternalIndex  = 0;

        Vertex *minVertex  = nullptr, *externalVertex = nullptr, *internalVertex = nullptr;
        ATile *currentTile = nullptr, *exitLoopTile   = nullptr, *minTile = nullptr;

        bool isOuter;
        bool ringClosed = false;
    private:
        int size, indexDistance;
    };

    class VertexBookkeeper
    {
    public:
        void AddOuterVertex(Vertex* vertex);
        void MarkAsSkipped(Vertex* vertex);
        void MarkAsProcessed(Vertex* vertex);
        void MarkAsAntVertex(Vertex* vertex);
        void AddRing();
        void Draw();

        bool IsUnprocessedOuterVertex(Vertex* vertex) const
        {
            return outerVertices.Contains(vertex);
        }
        bool IsProcessed(Vertex* vertex) const
        {
            for (auto vertexGroup : processedVertices)
                if (vertexGroup.Contains(vertex))
                    return true;
            return IsAntVertex(vertex);
        }
        bool HasBeenSkipped(Vertex* vertex) const
        {
            return skippedVertices.Contains(vertex);
        }
        bool IsAntVertex(Vertex* vertex) const
        {
            for (auto vertexGroup : antVertices)
                if (vertexGroup.Contains(vertex))
                    return true;
            return false;
        }
        Vertex* GetMinimumOuterVertex() const;

        Vertex* GetLastAntVertex() const
        {
            if (antVertices.Num() == 0 || antVertices.Last().Num() == 0)
                return nullptr;
            return antVertices.Last().Last();
        }
        int OuterVerticesNum()             const  { return outerVertices.Num(); }
        TArray<Vertex*> GetOuterVertices() const  { return outerVertices; }
        TArray<TArray<Vertex*>>& GetAntVertices() { return antVertices; }
        // TArray<Vertex*>& GetAntVertices()        { return antVertices.Last(); }

        // TODO::Make private
        TArray<Vertex*> skippedVertices;
    private:
        // All vertices that boarder copied tiles
        // With triangular boards, this main contain vertices that need to be skipped
        TArray<Vertex*> outerVertices;

        // Vertices that were inbetween two of their neighbors, as we only care about the vertices on the start and end
        
        // Vertices that remain after being skipped, but aren't eligiable to be an ant vertex
        TArray<TArray<Vertex*>> processedVertices;

        // These vertices, orginized by vertex rings, will be used as start/end points for the lines of the ant march
        TArray<TArray<Vertex*>> antVertices;
    };
};