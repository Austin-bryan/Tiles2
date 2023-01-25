#pragma once
#include "CoreMinimal.h"
#include "Board.h"
#include "CopySignature.h"
#include "GameFramework/Actor.h"
#include "ForwardDeclares.h"
#include "Tile.generated.h"

// #define SHOW_DEBUG_TEXT

#ifdef SHOW_DEBUG_TEXT
// #define SHOW_COORD
#define SHOW_ID
#endif

class ABoard;
class UMeshGenerator;
class UBoxComponent;
class UTextRenderComponent;
class UProceduralMeshComponent;
class UMaterialInstanceDynamic;
class UMaterialInstanceConstant;
enum class EBoardShape;
enum class ETileColor : uint8;

// ReSharper disable once CppPolymorphicClassWithNonVirtualPublicDestructor
class ITileComponent
{
public:
	virtual void SetColor(
		const ETileColor color,
		bool propagate) { }
}; 

UCLASS()
class TILES2_API ATile : public AActor, public ITileComponent
{
	GENERATED_BODY()
public:	
	static void ResetTileCount() { tileCount = 0; }
	
	ATile();
	void Tick(float DeltaSeconds) override;
	void SetShape(const EBoardShape) const;
	void SetBoard(ABoard* newBoard);
	void SetCoord(FCoordPtr coord);
	void ApplyColor(ETileColor color, bool propagate);

	int ID()			 const { return id; }
	ABoard* Board()      const { return board; }
	FCoordPtr GetCoord() const { return Coord; }
	EBoardShape Shape()  const;
	CopySignature Copy() const;
	bool IsOnBoardEdge() const;
	
	void SetColor(const ETileColor color, bool propagate)
	override;

	ATile* GetNeighbor(EDirection direction) const;
	TArray<ATile*> GetNeighbors() const;
	virtual ETileColor GetColor() const;
	bool IsAdjacent(const ATile* other) const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		UTextRenderComponent* CoordText;
	UPROPERTY()
		UMeshGenerator* MeshGenerator;
protected:
	FCoordPtr Coord;
	
	UPROPERTY()
		ABoard* board;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent* ProcMesh; 
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		UStaticMeshComponent* Collider;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		UBoxComponent* Box;
	UPROPERTY(VisibleAnywhere)
		int id;
	void NotifyActorOnClicked(FKey buttonPressed) override;
private:
	ETileColor tileColor;
	static int tileCount;

	UPROPERTY() UMaterialInstanceDynamic* instance;
	void SetDebugText() const;
};