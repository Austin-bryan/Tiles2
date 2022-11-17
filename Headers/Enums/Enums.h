#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Enums.generated.h"

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

UENUM(Blueprintable)
enum class EModule : uint8 
{
	Normal, Gap, Wrap, Camo, TeleportWrap, MirrorWrap, CircuitWrap, Rotator, Link, Rift, 
	Bandaged, Iron, Cloud, Swap, WrapWall, CorrectCounter,
	NoSpawn
};