#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ForwardDeclares.h"
#include "Tile.generated.h"

class UBoxComponent;
class UTextRenderComponent;
class ABoard;
enum class EBoardShape;
enum class ETileColor;

UCLASS()
class TILES2_API ATile : public AActor
{
	GENERATED_BODY()
public:	
	static void ResetTileCount() { tileCount = 0; }
	
	ATile();
	void Tick    (float DeltaTime) override;
	void SetShape(const EBoardShape) const;
	void SetColor(const ETileColor color) const;
	void SetBoard(ABoard* newBoard);
	void SetCoord(FCoordPtr coord);

	int ID()			  const { return id; }
	ABoard* Board()			    { return board; }
	FCoordPtr GetCoord()  const { return Coord; }

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		UTextRenderComponent* CoordText;
protected:
	void BeginPlay() override;
	FCoordPtr Coord;
	ABoard* board;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		UBoxComponent* Box;
	void NotifyActorOnClicked(FKey buttonPressed) override;
private:
	static int tileCount;
	int id;
};