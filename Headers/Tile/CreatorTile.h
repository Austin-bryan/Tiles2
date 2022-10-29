#pragma once
#include "CoreMinimal.h"
#include "Logger.h"
#include "Tile.h"
#include "CreatorTile.generated.h"

UCLASS()
class TILES2_API ACreatorTile : public ATile
{
	GENERATED_BODY()
public:
	ACreatorTile();
	void Tick(float deltaSeconds) override;
	void NotifyActorOnClicked(FKey ButtonPressed) override;
private:
	bool isSelected, isPlayingSelectAnim;
	FVector targetScale, startScale;
	float selectAlpha;
};