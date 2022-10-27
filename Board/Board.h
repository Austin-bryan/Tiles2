#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
#include "ForwardDeclares.h"
#include "TilesMap.h"
#include "Board.generated.h"

struct FCoord;
class ATile;

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

	FCoordPtr GetSize()			const { return Size;  }
	EBoardShape GetBoardShape() const { return Shape; }
	FVector LocationOf(FCoordPtr coord) const;
	CoordMembers MinBounds() const;
	CoordMembers MaxBounds() const;
protected:
	void BeginPlay() override;
	EBoardShape Shape;

	UPROPERTY(EditAnywhere, BlueprintReadonly)
		FString BoardSeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		USceneComponent* Root;
	FCoordPtr Size;
private:
	static float GetCenteredPosition(const float coord);

	Tiles tiles;
};