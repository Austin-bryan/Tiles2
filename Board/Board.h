#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
#include "ForwardDeclares.h"
#include "Board.generated.h"

struct FCoord;
class ATile;

//todo== tiles should be private map that is passed by reference to populator and gettiles returns const reference to prevent permuatiion	
UCLASS()
class TILES2_API ABoard : public AActor
{
	GENERATED_BODY()
public:	
	ABoard();
	void Tick(float DeltaTime) override;
    
	FCoordPtr GetSize()			const { return Size; }
	const Tiles& GetTiles()     const { return tiles; }
	EBoardShape GetBoardShape() const { return Shape; }

	FVector LocationOf(FCoordPtr coord) const;
protected:
	void BeginPlay() override;
	EBoardShape Shape;

	UPROPERTY(EditAnywhere, BlueprintReadonly)
		FString BoardSeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Z;

	FCoordPtr Size;
private:
	static float GetCenteredPosition(const float coord);
	Tiles tiles;
};