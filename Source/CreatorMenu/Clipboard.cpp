// ReSharper disable All
#include "Clipboard.h"
#include "CreatorTile.h"

void Clipboard::Copy()
{
    copiedMap.Empty();
    copiedTiles.Empty();

    if (ACreatorTile::SelectedTiles().Num() == 0)
    {
        Log("No Tiles Selected", RED);
        return;
    }
    Log("Copy");
    oldFirstCoord = ACreatorTile::SelectedTiles()[0]->GetCoord();
    
    for (auto selectedTile : ACreatorTile::SelectedTiles()) 
        copiedMap.Add(*selectedTile->GetCoord(), CopySignature(selectedTile));
    AntMarchSpawner.HideLines();
    AntMarchSpawner.Draw(copiedMap);
}
void Clipboard::Cut() { }

void Clipboard::Paste()
{
    if (copiedMap.Num() == 0 || ACreatorTile::FirstSelectedTile() == nullptr)
    {
        Log(copiedTiles.Num() == 0, ACreatorTile::FirstSelectedTile() == nullptr, RED);
        return;
    }
    auto creatorBoard = ACreatorTile::SelectedTiles()[0]->Board();

    TilesMap tilesToReplace;
    for (auto selectedTile : ACreatorTile::SelectedTiles()) 
        tilesToReplace.Add(selectedTile->GetCoord(), selectedTile);
    
    FVector copiedMax = FVector(INT_MIN);
    FVector copiedMin = FVector(INT_MAX);
    FVector pasteMin  = FVector(INT_MAX);
    FCoordPtr firstCoord = ACreatorTile::SelectedTiles()[0]->GetCoord();

    const auto MinMax = [](FVector& a, FCoord& b, float (*func)(float, float))
    {
        a.X = func(a.X, b.X());
        a.Y = func(a.Y, b.Y());
        a.Z = func(a.Z, b.Z());
    };

    for (auto [coord, _] : copiedMap)
    {
        MinMax(copiedMin, coord, &FMath::Min);
        MinMax(copiedMax, coord, &FMath::Max);
    }
    for (auto tile : ACreatorTile::SelectedTiles())
    {
        FCoord coord = *tile->GetCoord();
        MinMax(pasteMin, coord, &FMath::Min);
    }
    
    const auto GetDistance = [](float start, double end) { return (float)(end - start); };
    auto distances = FVector(GetDistance(copiedMin.X, pasteMin.X), GetDistance(copiedMin.Y, pasteMin.Y), GetDistance(copiedMin.Z, pasteMin.Z));
    auto lengths   = FVector(FMath::Abs(GetDistance(copiedMin.X, copiedMax.X) + 1), FMath::Abs(GetDistance(copiedMin.Y, copiedMax.Y) + 1), FMath::Abs(GetDistance(copiedMin.Z, copiedMax.Z) + 1));

    for (auto tile : ACreatorTile::SelectedTiles())
    {
        auto tileDistances = distances - FVector(GetDistance(copiedMin.X, tile->GetCoord()->X()), GetDistance(copiedMin.Y, tile->GetCoord()->Y()), GetDistance(copiedMin.Z, tile->GetCoord()->Z()));
        FVector halfLength =  FVector(                          
            FMath::TruncToDouble(abs(fmod(tileDistances.X, lengths.X))), 1,
            FMath::TruncToDouble(abs(fmod(tileDistances.Z, lengths.Z))));
        FCoordPtr coord = tile->GetCoord() - FCoordPtr(new FCoord(halfLength.X, halfLength.Y, halfLength.Z));

        coord = MakeShared<const FCoord>(copiedMin.X + halfLength.X, 0, copiedMin.Z + halfLength.Z);
        if (copiedMap.Contains(*coord))
            tile->SetColor(copiedMap[*coord].Color(), true);
    }
}
void Clipboard::Tick() { }
void Clipboard::BindToSeedChange(ASeedCreator* seedCreator) { AntMarchSpawner.BindToSeedChange(seedCreator); }