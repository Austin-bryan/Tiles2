#pragma once
#include "Logger.h"
#include "CoreMinimal.h"

// TODO:: make this constants, then rename them to normal naming convention
#define SQUARE_TILE        "/Game/Meshes/SM_SquareTile.SM_SquareTile"
#define SIMPLE_SQUARE      "/Game/Meshes/SM_SquareSimple.SM_SquareSimple"
#define HEX_TILE           "/Game/Meshes/SM_HexTile.SM_HexTile"
#define TRIANGLE_TILE      "/Game/Meshes/SM_TriangleTile.SM_TriangleTile"
#define MAT_SELECTION_BOX  "/Game/Materials/Mat_SelectionBody.Mat_SelectionBody"
#define SELECTION_CYLINDER "/Game/Meshes/SM_SelectionCylinder.SM_SelectionCylinder"
#define SQUARE_SPRITE      "/Game/Sprites/SquareFlat_Sprite.SquareFlat_Sprite"

template<typename T>
static FORCEINLINE T* LoadObjectFromPath(const FString& path)
{
    return Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *path));
}
static FORCEINLINE UMaterial* LoadMaterialFromPath(const FString& path) { return LoadObjectFromPath<UMaterial>(path); }