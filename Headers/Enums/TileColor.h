#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TileColor.generated.h"

UENUM(Blueprintable)
enum class ETileColor : uint8
{
	None UMETA(DisplayName = "None"), White, Red, Orange, Yellow, Green,
	Cyan, Blue, Purple, Pink, Magenta, Black,
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

UCLASS()
class UColorCast : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, meta=( DisplayName = "To Linear Color", CompactNodeTitle = "•"), Category = "TilesEnums")
	static FLinearColor TileColorToLinearColor(const ETileColor color);

	UFUNCTION(BlueprintCallable, meta=( DisplayName = "Color Creator Tiles"), Category = "Creator Board")
	static void ColorCreatorTiles(ETileColor color);
};
bool operator==(ETileColor lhs, ETileColor rhs);
bool operator!=(const ETileColor lhs, const ETileColor rhs);
