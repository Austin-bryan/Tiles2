#pragma once
#include "CoreMinimal.h"

UENUM()
enum class EBoardShape
{
	Square   UMETA(DisplayName = "Square"),
	Hex      UMETA(DisplayName = "Hex"),
	Triangle UMETA(DisplayName = "Triangle"),
};
enum class EDirection
{
	Up, Down, Left, Right,
	UpLeft, UpRight, DownLeft, DownRight
};
enum class ETileColor
{
	None, White, Red, Orange, Yellow, Green, 
	Cyan, Blue, Purple, Pink, Magenta, Black
};
enum class EModule
{
	Normal, Gap, Wrap, Camo, TeleportWrap, MirrorWrap, CircuitWrap, Rotator, Link, Rift, 
	Bandaged, Iron, Cloud, Swap, WrapWall, CorrectCounter,
	NoSpawn
};
enum class EBoardParseState
{
	Default, Module, Parameter, Number, String, Coord, Last
};
enum class EParameter
{
	Void, Number, String, Coord, SqrCoord, TriCoord, HexCoord
};

inline TMap<ETileColor, FString> TileColorStrings =
{
	{ ETileColor::None,    FString("None") },
	{ ETileColor::White,   FString("White") },
	{ ETileColor::Red,     FString("Red") },
	{ ETileColor::Orange,  FString("Orange") },
	{ ETileColor::Yellow,  FString("Yellow") },
	{ ETileColor::Green,   FString("Green") },
	{ ETileColor::Cyan,    FString("Cyan") },
	{ ETileColor::Blue,    FString("Blue") },
	{ ETileColor::Purple,  FString("Purple") },
	{ ETileColor::Pink,    FString("Pink") },
	{ ETileColor::Magenta, FString("Magenta") },
	{ ETileColor::Black,   FString("Black") }
};