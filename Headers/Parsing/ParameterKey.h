#pragma once
#include "CoreMinimal.h"
enum class EBoardShape;

/*
 * Maintains the parameter list for all tile modules.
 * Some modules have overloads, such as BandagaeModule,
 * so this selects which overload to use as well.
 */
class TILES2_API ParameterKey
{
public:
	ParameterKey(){ }
	explicit ParameterKey(const EBoardShape boardShape);

	bool Contains(const FString& text) const;
	FString& operator[](const FString& index);
	FString* Find(const FString& parsedText);
private:
	TMap<FString, FString> key;
	FString format(const std::initializer_list<FString> args) const;
};
