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
	Void, Number, String, Coord
};