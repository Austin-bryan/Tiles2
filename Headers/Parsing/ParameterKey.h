#pragma once
#include "CoreMinimal.h"
enum class EBoardShape;

class TILES2_API ParameterKey
{
public:
	ParameterKey(){ }
	explicit ParameterKey(const EBoardShape boardShape);

	bool Contains(const FString& text);
	FString& operator[](const FString& index);
	FString* Find(const FString& parsedText);
private:
	TMap<FString, FString> key;
	FString format(const std::initializer_list<FString> args);
};
