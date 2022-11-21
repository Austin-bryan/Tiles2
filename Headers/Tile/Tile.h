#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ForwardDeclares.h"
#include "Tile.generated.h"

class UMaterialInstanceConstant;
class UBoxComponent;
class UTextRenderComponent;
class ABoard;
class UMeshGenerator;
enum class EBoardShape;
enum class ETileColor : uint8;

UCLASS()
class TILES2_API ATile : public AActor
{
	GENERATED_BODY()
public:	
	static void ResetTileCount() { tileCount = 0; }
	
	ATile();
	void Tick    (float DeltaTime) override;
	void SetShape(const EBoardShape) const;
	void SetColor(const ETileColor color);
	void SetBoard(ABoard* newBoard);
	void SetCoord(FCoordPtr coord);

	ABoard* Board()       const { return board; }
	int ID()			  const { return id; }
	FCoordPtr GetCoord()  const { return Coord; }

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		UTextRenderComponent* CoordText;
protected:
	void BeginPlay() override;
	FCoordPtr Coord;
	
	UPROPERTY()
		ABoard* board;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UMeshGenerator* MeshGenerator;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		UStaticMeshComponent* Collider;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		UBoxComponent* Box;
	void NotifyActorOnClicked(FKey buttonPressed) override;
private:
	static int tileCount;
	int id;
	
	UPROPERTY()
		UMaterialInstanceDynamic* instance;
};
