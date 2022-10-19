#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
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
	void Tick(float DeltaTime) override;
	void SetShape(const EBoardShape) const;
	void SetColor(const ETileColor color) const;

	FCoordPtr GetCoord() const { return Coord; }
	void SetCoord(FCoordPtr coord);
	static TMap<ETileColor, FString> TileColorStrings;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		UTextRenderComponent* CoordText;
protected:
	void BeginPlay() override;
	FCoordPtr Coord;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadonly)
		int rot;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
		UBoxComponent* Box;
	void NotifyActorOnClicked(FKey buttonPressed) override;
};
