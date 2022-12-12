#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ForwardDeclares.h"
#include "Tile.generated.h"

#define ShowDebugText
// #define ShowCoord
#define ShowID

class ABoard;
class UMeshGenerator;
class UBoxComponent;
class UTextRenderComponent;
class UProceduralMeshComponent;
class UMaterialInstanceConstant;
enum class EBoardShape;
enum class ETileColor : uint8;

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
	void Tick(float DeltaTime) override;

	void SetShape(const EBoardShape) const;
	void SetBoard(ABoard* newBoard);
	void SetCoord(FCoordPtr coord);

	int ID()			  const { return id; }
	ABoard* Board()       const { return board; }
	FCoordPtr GetCoord()  const { return Coord; }
	void SetColor(const ETileColor color,
		bool propagate) override;

	TArray<ATile*> GetAdjacent() const;
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
	void NotifyActorOnClicked(FKey buttonPressed) override;
private:
	int id;
	ETileColor tileColor;
	static int tileCount;
	
	UPROPERTY()
		UMaterialInstanceDynamic* instance;
};