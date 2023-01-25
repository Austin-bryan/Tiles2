#include "SeedCreator.h"

#include "AssetDir.h"
#include "CreatorBoard.h"
#include "CreatorTile.h"
#include "PaperSpriteComponent.h"
#include "Parser.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"

ASeedCreator::ASeedCreator()
{
    PrimaryActorTick.bCanEverTick = true;
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);
    
    Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
    Sprite->SetupAttachment(Root);
    Sprite->SetRelativeScale3D(FVector(3.f, 1, 0.16f));
    Sprite->SetSpriteColor(FColor::Black);
    Sprite->SetSprite(LoadObjectFromPath<UPaperSprite>(SQUARE_SPRITE));
    
    TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
    TextRender->SetupAttachment(Root);
    TextRender->SetWorldSize(20);
    TextRender->SetHorizontalAlignment(EHTA_Center);
    TextRender->SetVerticalAlignment(EVRTA_TextCenter);
    TextRender->SetRelativeRotation(FRotator(0, 90, 0));
    TextRender->SetRelativeLocation(FVector(0, 1, 0));
    TextRender->SetTextRenderColor(FColor::White);

    // Moved out of the way for now:
    SetActorLocation(FVector(0, -270, 720));
    // SetActorLocation(FVector(0, -270, 360));
}
void ASeedCreator::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (UGameplayStatics::GetTimeSeconds(GetWorld()) - lastUpdate <= 0.25f)
        return;
    FString seed = GetBoardSeed() + " | "_f + GetTileSide();
    
    if (seed != lastSeed && OnChangeDetected.IsBound())
        OnChangeDetected.Execute();
    SetText(seed);
    lastSeed = seed;
}

void ASeedCreator::SetText(FString text) const
{
    const int wrapLimit = 140;
    int loopCount = 0;          
    FString finalText;

    do finalText += text.Mid(wrapLimit * loopCount++, wrapLimit) + "\n"_f;
    while (text.Len() > wrapLimit * loopCount);

    Sprite->SetRelativeScale3D(FVector(3.f, 1, 0.04 * loopCount));
    TextRender->SetText(ftxt(finalText));
}
void ASeedCreator::SetBoard(ACreatorBoard* _creatorBoard)
{
    creatorBoard = _creatorBoard;
    AttachToActor(creatorBoard, FAttachmentTransformRules::KeepWorldTransform);
}
FString ASeedCreator::GetBoardSeed() const { return creatorBoard->GetSize()->ToString(); }
FString ASeedCreator::GetTileSide() const
{
    FString seed;

    for (auto [_, tile] : creatorBoard->GetTiles())
        seed += Parser::ReverseParseTile(Cast<ACreatorTile>(tile)) + "/"_f;        

    // for (int i = 0; i < creatorBoard->GetTiles().Num(); i++)
        // seed += Parser::ReverseParseTile(Cast<ACreatorTile>(creatorBoard->GetTiles().Values()[i])) + "/"_f;
    seed += ";"_f;
    return seed.Replace(*"/;"_f, *";"_f);
}