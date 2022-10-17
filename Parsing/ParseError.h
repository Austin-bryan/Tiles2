#pragma once
#include "CoreMinimal.h"
#include "Logger.h"
#include "Parsing/LexerPosition.h"

/**
 * Prints errors for seed parsing, showing the context and uses LexerPosition to get column and tile count
 */
class TILES2_API ParseError
{
public:
	explicit ParseError(FString& seed) : seed{ seed } { }
	void Throw(const TUniquePtr<LexerPosition>& position, FString error, const FString& expected, const FString& state) const;
protected:
	FString seed;
};
