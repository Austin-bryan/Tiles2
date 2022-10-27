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

// struct CoordMembers
// {
//     float X, Y, Z;
//     CoordMembers(const float x, const float y, const float z): X{ x }, Y{ y }, Z{ z } { }
//     
//     bool operator<(CoordMembers rhs)
//     {
//         return X < rhs.X || Y < rhs.Y || Z < rhs.Z;
//     }
// };