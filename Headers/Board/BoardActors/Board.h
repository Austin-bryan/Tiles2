#pragma once
#include "TilesMap.h"
#include "BoardSeed.h"
#include "CoreMinimal.h"
#include "ForwardDeclares.h"
#include "GameFramework/Actor.h"
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

	ATile*& At(const FCoordPtr index)          { return tiles[index]; }
	ATile*& At(const FCoord& index)            { return tiles[index]; }
	ATile*& operator[](const FCoordPtr index)  { return tiles[index]; }
	ATile*& operator[](const FCoord& index)	   { return tiles[index]; }

	ATile*  At(const FCoordPtr index) 		  const { return tiles.Contains(index) ? tiles[index] : nullptr; }
	ATile*  At(const FCoord& index)   		  const { return tiles.Contains(index) ? tiles[index] : nullptr; }
	ATile*  operator[](const FCoordPtr index) const { return tiles.Contains(index) ? tiles[index] : nullptr; }
	ATile*  operator[](const FCoord& index)   const { return tiles.Contains(index) ? tiles[index] : nullptr; }

	FCoordPtr GetSize()			   const { return Size;  }
	const TilesMap& GetTiles() 	   const { return tiles; }
	EBoardShape GetBoardShape()    const { return Shape; }
	bool Contains(FCoordPtr coord) const { return tiles.Contains(coord); }

	FVector LocationOf(FCoordPtr coord) const;
	FCoord MinBounds() const;
	FCoord MaxBounds() const;
	
	virtual UClass* TileClass() const { return nullptr; }
protected:
	void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		USceneComponent* Root;
		
	UPROPERTY(EditInstanceOnly, Category="Seed")
		bool UseCustomSeed;
	UPROPERTY(EditInstanceOnly, Category="Seed", meta=(EditCondition="!UseCustomSeed", EditConditionHides))
		EBoardShape Shape = EBoardShape::Square;
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category="Seed", meta=(EditCondition="UseCustomSeed", EditConditionHides))
		FString BoardSeed;
	
	UPROPERTY(EditInstanceOnly, Category="Seed", meta=(EditCondition="!UseCustomSeed && Shape == EBoardShape::Hex", EditConditionHides))
		EHexSeed HexSeed = EHexSeed::Equal3;
	UPROPERTY(EditInstanceOnly, Category="Seed", meta=(EditCondition="!UseCustomSeed && Shape == EBoardShape::Square", EditConditionHides))
		ESqrSeed SquareSeed = ESqrSeed::Equal3;
	UPROPERTY(EditInstanceOnly, Category="Seed",meta=(EditCondition="!UseCustomSeed && Shape == EBoardShape::Triangle", EditConditionHides))
		ETriSeed TriangleSeed = ETriSeed::Equal3;

	FCoordPtr Size;
	TilesMap tiles;
private:
	static float GetCenteredPosition(const float coord);
	void OnConstruction(const FTransform& Transform) override;
};