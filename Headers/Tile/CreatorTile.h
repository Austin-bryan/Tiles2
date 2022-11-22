#pragma once
#include "CoreMinimal.h"
#include "ActorAnimator.h"
#include "Tile.h"
#include "MeshGenerator.h"
#include "CreatorTile.generated.h"


/**
 * CreatorTiles are used in the level creator and have additional features such as being able to be selected.
 */
UCLASS()
class TILES2_API ACreatorTile : public ATile
{
	GENERATED_BODY()
public:
	// TODO:: make private with const getter
	static TArray<ACreatorTile*> SelectedTiles;
	static void EmptySelectedTiles();
	static ACreatorTile* FirstSelectedTile() { return firstSelectedTile; }

	ACreatorTile();
	bool GetIsSelected() const { return isSelected; }
	void Tick(float deltaSeconds) override;
	void NotifyActorOnClicked(FKey ButtonPressed) override;
	void Select(bool _isSelected, bool isDragSelecting = false);
private:
	static ACreatorTile* firstSelectedTile;
	bool isSelected, isPlayingSelectAnim;
	FVector targetScale, startScale;
	float selectAlpha;
	bool wasDragSelected;

	ActorAnimator* activeAnimation;
	ActorAnimator animPress{ProcMesh, FVector::One(), FVector(0.75f), EAnimMode::Scale, 8};
	ActorAnimator animHover{ProcMesh, FVector::One(), FVector(1.05f), EAnimMode::Scale, 8};
};