#pragma once
#include "CoreMinimal.h"
#include "ActorAnimator.h"
#include "Tile.h"
#include "CreatorTile.generated.h"

UCLASS()
class TILES2_API ACreatorTile : public ATile
{
	GENERATED_BODY()
public:
	static TArray<ACreatorTile*> SelectedTiles;
	static void EmptySelectedTiles();

	ACreatorTile();
	void Tick(float deltaSeconds) override;
	void NotifyActorOnClicked(FKey ButtonPressed) override;
	void NotifyActorBeginCursorOver() override;
	void NotifyActorEndCursorOver() override;

	void Select();
	void Deselect();
private:
	bool isSelected, isPlayingSelectAnim;
	FVector targetScale, startScale;
	float selectAlpha;

	ActorAnimator* activeAnimation;
	ActorAnimator animPress = ActorAnimator(this, FVector::One(), FVector(0.75f), EAnimMode::Scale, 8);
	ActorAnimator animHover = ActorAnimator(this, FVector::One(), FVector(1.05f), EAnimMode::Scale, 8);
};