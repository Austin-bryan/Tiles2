#include "StaticResetter.h"

#include "BandagedModule.h"
#include "CreatorTile.h"
#include "Vertex.h"

void AStaticResetter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (!(EndPlayReason == EEndPlayReason::EndPlayInEditor))
        return;
    Super::EndPlay(EndPlayReason);
    Vertex::Vertices.Empty();
    ACreatorTile::selectedTiles.Empty();
    ABandagedModule::bandageCount = 0;
}