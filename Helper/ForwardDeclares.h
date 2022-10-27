#pragma once

class ATile;
struct FCoord;
struct FSqrCoord;
struct FHexCoord;
struct FTriCoord;

using FCoordPtr    = TSharedPtr<const FCoord>;
using FSqrCoordPtr = TSharedPtr<const FSqrCoord>;
using FHexCoordPtr = TSharedPtr<const FHexCoord>;
using FTriCoordPtr = TSharedPtr<const FTriCoord>;
using CoordMembers = TTuple<float, float, float>;