#include "DefaultParseState.h"
#include "Parsing/Token.h"
#include "Parser.h"

DefaultParseState::DefaultParseState(Parser& parser, TSharedPtr<ParseState> parent) : ParseState(parser, parent)
{
}
DefaultParseState::~DefaultParseState() { }
void DefaultParseState::ParseAlpha(const char c)
{
	if (TileColorParseKey.Contains(c))
	{
		if (currentColor == ETileColor::None)
		{
			currentColor = TileColorParseKey[c];
			CurrentTile()->SetColor(currentColor);
		}
		else parser.Throw(c, "/");
	}
	else parser.Throw(c, "Color expected");
}
void DefaultParseState::ParseSeparator()
{
	if (currentColor == ETileColor::None)
		 parser.Throw(TSeparator, "Color expected");
	else Reset();
}
void DefaultParseState::ParseSemiColon()
{
	if (currentColor == ETileColor::None)	
		parser.Throw(TSemiColon, "Color expected");
}

FString DefaultParseState::Name() const { return "Default State"; }
void DefaultParseState::Reset() { currentColor = ETileColor::None; }
void DefaultParseState::ParseLeftBrace()
{
	if (currentColor != ETileColor::None)
	 	 PushState(EBoardParseState::Module);
	else parser.Throw(TRightBrace, "Color expected");
}

FString DefaultParseState::GetExpectedMessage()
{
	return fstr(currentColor == ETileColor::None ? "Color Identifier" : "/");
}