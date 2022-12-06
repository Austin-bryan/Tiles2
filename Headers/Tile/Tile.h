#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ForwardDeclares.h"
#include "Tile.generated.h"

//#define ShowDebugText

class ABoard;
class UMeshGenerator;
class UBoxComponent;
class UTextRenderComponent;
class UProceduralMeshComponent;
class UMaterialInstanceConstant;
enum class EBoardShape;
enum class ETileColor : uint8;

UCLASS()
class TILES2_API ATile : public AActor
{
	GENERATED_BODY()
public:	
	static void ResetTileCount() { tileCount = 0; }
	
	ATile();
	void Tick(float DeltaTime) override;

	void SetShape(const EBoardShape) const;
	virtual void SetColor(
		const ETileColor color,
		bool colorSiblings = true);
	void SetBoard(ABoard* newBoard);
	void SetCoord(FCoordPtr coord);
	void BandagedWith(
		TSharedPtr<TArray<ATile*>> sharedSiblings);

	int ID()			  const { return id; }
	ABoard* Board()       const { return board; }
	FCoordPtr GetCoord()  const { return Coord; }
	virtual ETileColor GetColor() const;
	
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
	TSharedPtr<TArray<ATile*>> siblings;
private:
	static int tileCount;
	int id;
	ETileColor tileColor;
	
	UPROPERTY()
		UMaterialInstanceDynamic* instance;
};