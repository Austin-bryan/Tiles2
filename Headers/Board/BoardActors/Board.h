#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TilesMap.h"
#include "ForwardDeclares.h"
#include "Board.generated.h"

class ATile;
enum class EBoardShape;

UCLASS()
class TILES2_API ABoard : public AActor
{
	GENERATED_BODY()
public:	
	ABoard();
	void Tick(float deltaTime) override;
	
	ATile*&       At(const FCoordPtr index)       { return tiles[index]; }
	ATile* const& At(const FCoordPtr index) const { return tiles[index]; }

	Tiles& GetTiles() { return tiles; }

	FCoordPtr GetSize() const { return Size;  }
	EBoardShape GetBoardShape() const { return Shape; }
	FVector LocationOf(FCoordPtr coord) const;
	FCoord MinBounds() const;
	FCoord MaxBounds() const;
	
	virtual UClass* TileClass() const { return nullptr; }
protected:
	void BeginPlay() override;
	EBoardShape Shape;

	UPROPERTY(EditAnywhere, BlueprintReadonly)
		FString BoardSeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		USceneComponent* Root;
	FCoordPtr Size;
	Tiles tiles;
private:
	static float GetCenteredPosition(const float coord);
};