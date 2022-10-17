#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
#include "Parser.h"
#include "Board.generated.h"

struct FCoord;
class ATile;

using FTiles = TMap<FCoord*, ATile*>;

//todo:= tiles should be private map that is passed by reference to populator and gettiles returns const reference to prevent permuatiion	
UCLASS()
class TILES2_API ABoard : public AActor
{
	GENERATED_BODY()
public:	
	ABoard();
	virtual void Tick(float DeltaTime) override;
    
	const FCoord* GetSize()  const { return Size; }
	const FTiles& GetTiles() const { return tiles; }
	EBoardShape GetBoardShape() const { return Shape; }
protected:
	virtual void BeginPlay() override;

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

	FCoord* Size;
private:
	static float GetCenteredPosition(const float coord);
	FTiles tiles;
	//TOptional<Parser> parser;
};