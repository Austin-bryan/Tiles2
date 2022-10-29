#include "ParseError.h"
#include "Logger.h"

void ParseError::Throw(const TUniquePtr<LexerPosition>& position, const FString error, const FString& expected, const FString& state) const
{
	int startContext = position->GetIndex() - 5;
	int endContext   = position->GetIndex() + 6;

	if (startContext < 0)
		startContext = 0;
	if (endContext > seed.Len())
		endContext = seed.Len();

	char context[12];
	const char* cText = StringCast<ANSICHAR>(*seed).Get();

	for (int j = 0, i = startContext; i < endContext; j++, i++)
		context[j] = cText[i];
	context[11] = '\0';

	/*
		Invalid Character 'error' at Column: i, Tile: t
		Context: context;
		Expected: 'expected'
		Staete: <name> Parse State.
	*/
	const FString message  = fstr("Invalid Character ") + fstr("\'") + error + fstr("\'") + fstr(" at Column:  ") + fstr(position->GetIndex()) + ", Tile: " + fstr(position->GetTile());
	const FString fContext = fstr("Context: ") + fstr(context);
	const FString stateText = fstr("Parse State: ") + state;

	Log(message + NL + fContext + NL + expected + NL + stateText, FColor::Red);
}