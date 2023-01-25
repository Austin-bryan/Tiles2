#pragma once
#include "CoreMinimal.h"

class ATile;
enum class ETileColor : uint8;

struct CopySignature
{
    explicit CopySignature(ATile* _tile);

    ATile* GetTile()    const { return tile; }
    ETileColor Color()  const { return color; }

    bool operator==(CopySignature rhs) const
    {
        if (tile != rhs.tile)
            return false;
        if (color != rhs.color)
            return false;
        return true;
    }
    bool operator!=(CopySignature rhs) const { return !(*this == rhs); }

    ATile* tile;
    ETileColor color;
};