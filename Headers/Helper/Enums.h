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
enum class ETileColor : uint8
{
	None, White, Red, Orange, Yellow, Green,
	Cyan, Blue, Purple, Pink, Magenta, Black,
};
enum class ETest
{
	Swag, Foo
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

inline uint8 GetTypeHash(const ETileColor e) { return static_cast<uint8>(e); }

// todod:: move tilecolor into seperate class
UCLASS()
class UColorCast : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
UFUNCTION(BlueprintPure, meta=( DisplayName = "To Linear Color", CompactNodeTitle = "•"), Category = "TilesEnums")
static FLinearColor TileColorToLinearColor(const ETileColor color)
{
	switch(color)
	{
	case ETileColor::None:    return FLinearColor(0, 0, 0, 0);
	case ETileColor::White:   return FLinearColor(0.9f, 0.9f, 0.9f, 1.0f);
	case ETileColor::Red:     return FLinearColor(0.8, 0, 0, 1);
	case ETileColor::Orange:  return FLinearColor(1, 0.15f, 0, 1);
	case ETileColor::Yellow:  return FLinearColor(0.6f, 0.6f, 0, 1);
	case ETileColor::Green:   return FLinearColor(0, 0.4f, 0, 1);
	case ETileColor::Cyan:    return FLinearColor(0.02f, 0.35f, 0.6f, 1);
	case ETileColor::Blue:    return FLinearColor(0, 0.07f, 1.0f, 1);
	case ETileColor::Purple:  return FLinearColor(0.2f, 0, 0.3f, 1);
	case ETileColor::Pink:    return FLinearColor(0.65f, 0, 0.1f, 1);
	case ETileColor::Magenta: return FLinearColor(0.6f, 0, 0.6f, 1);
	case ETileColor::Black:   return FLinearColor(0.025f, 0.025f, 0.025f, 1);
	default: throw std::invalid_argument("Invalid tile color enum.");
	}
}
};
inline bool operator==(ETileColor lhs, ETileColor rhs)
{
	return static_cast<int>(lhs) == static_cast<int>(rhs);
}
inline bool operator!=(const ETileColor lhs, const ETileColor rhs)
{
	return !(lhs == rhs);
}