#pragma once
#include "CoreMinimal.h"
enum class EBoardShape;

// TODO:: This class might not be needed.
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
