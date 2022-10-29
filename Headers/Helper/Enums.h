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
UENUM(BlueprintType)
enum class ETileColor : uint8
{
	None    UMETA(DisplayName = "None"),
	White   UMETA(DisplayName = "White"),
	Red     UMETA(DisplayName = "Red"),
	Orange  UMETA(DisplayName = "Orange"),
	Yellow  UMETA(DisplayName = "Yellow"),
	Green   UMETA(DisplayName = "Green"), 
	Cyan    UMETA(DisplayName = "Cyan"),
	Blue    UMETA(DisplayName = "Blue"),
	Purple  UMETA(DisplayName = "Purple"),
	Pink    UMETA(DisplayName = "Pink"),
	Magenta UMETA(DisplayName = "Magenta"),
	Black   UMETA(DisplayName = "Black")
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

inline bool operator==(ETileColor lhs, ETileColor rhs)
{
	return static_cast<int>(lhs) == static_cast<int>(rhs);
}
inline bool operator!=(ETileColor lhs, ETileColor rhs)
{
	return !(lhs == rhs);
}
