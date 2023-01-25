#pragma once
#include "CoreMinimal.h"
#include "AntMarchSpawner.h"
#include "ForwardDeclares.h"
#include "TilesMap.h"

class ASeedCreator;
class AntMarchSpawner;
class ACreatorTile;

class Clipboard
{
public:
    explicit Clipboard(ACreatorBoard* creatorBoard) : AntMarchSpawner{ creatorBoard } {  }
    void Copy();
    void Cut();
    void Paste();
    void Tick();
    void BindToSeedChange(ASeedCreator* seedCreator);
private:
    TArray<CopySignature> copiedTiles;
    TMap<FCoord, CopySignature> copiedMap;
    FCoordPtr oldFirstCoord;

    AntMarchSpawner AntMarchSpawner;
};