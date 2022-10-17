#include "LexerPosition.h"
#include "Parsing/Token.h"
#include "Logger.h"

/**
 * Maintains position, column and tile count of parser
 */
LexerPosition::LexerPosition(FString seed) : seed{ seed } { index = 0; }
void LexerPosition::Next()
{
	index++;
	if (index < seed.Len())
	{
		auto currentChar = seed[index];
		if (currentChar == TSeparator)
			tile++;
	}
	else Log("Error: Index out of Bounds. LexerPosition.cpp Line 13"); 
}
void LexerPosition::Reset()
{
	index = 0;
	tile = 1;
}