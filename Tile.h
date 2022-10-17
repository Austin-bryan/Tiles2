#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
#include "AssetDir.h"
#include "Coord/Coord.h"
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"
#include "Tile.generated.h"

UCLASS()
class TILES2_API ATile : public AActor
{
	GENERATED_BODY()
public:	
	ATile();
	virtual void Tick(float DeltaTime) override;
	void SetShape(EBoardShape);
	void SetColor(ETileColor color);

	FCoord* GetCoord() const { return Coord; }
	void SetCoord(FCoord* coord);
	static TMap<ETileColor, FString> TileColorStrings;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		UTextRenderComponent* CoordText;
protected:
	virtual void BeginPlay() override;
	FCoord* Coord;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		UStaticMeshComponent* Mesh;
	//UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		//UStaticMeshComponent* m;
	UPROPERTY(EditAnywhere, BlueprintReadonly)
		int rot;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		UBoxComponent* Box;
	void NotifyActorOnClicked(FKey buttonPressed) override;
};
