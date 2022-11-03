#pragma once

#define SQUARE_TILE       "/Game/Meshes/SM_SquareTile.SM_SquareTile"
#define HEX_TILE          "/Game/Meshes/SM_HexTile.SM_HexTile"
#define TRIANGLE_TILE     "/Game/Meshes/SM_TriangleTile.SM_TriangleTile"
#define MAT_SELECTION_BOX "/Game/Materials/Mat_SelectionBody.Mat_SelectionBody"
#include "Logger.h"

template<typename T>
static FORCEINLINE T* LoadObjectFromPath(const FName& path)
{
    if (path == NAME_None)
    {
        Log("Invalid name.");
        return nullptr;
    }
    return Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *path.ToString()));
}

static FORCEINLINE UMaterial* LoadMaterialFromPath(const FName& path)
{
    return LoadObjectFromPath<UMaterial>(path);
}