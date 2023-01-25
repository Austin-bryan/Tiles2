#pragma once
#include "CoreMinimal.h"
#include "SeedCreator.generated.h"

class ACreatorBoard;
class UTextRenderComponent;
class UPaperSpriteComponent;

UCLASS()
class ASeedCreator : public AActor
{
    GENERATED_BODY()
public:
    ASeedCreator();

    void Tick(float DeltaSeconds) override;
    void SetText(FString text) const;
    void SetBoard(ACreatorBoard* _creatorBoard);

    DECLARE_DELEGATE(FOnChangeDetectedSignature);
    FOnChangeDetectedSignature OnChangeDetected;
protected:
    UPROPERTY(VisibleAnywhere) USceneComponent* Root;
    UPROPERTY(VisibleAnywhere) UPaperSpriteComponent* Sprite;
    UPROPERTY(VisibleAnywhere) UTextRenderComponent* TextRender;
private:
    ACreatorBoard* creatorBoard;
    
    FString GetBoardSeed() const;
    FString GetTileSide() const;
    float lastUpdate = 0;
    FString lastSeed;
};