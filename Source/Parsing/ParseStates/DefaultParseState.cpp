#include "DefaultParseState.h"

#include "Logger.h"
#include "Token.h"
#include "Parser.h"
#include "Tile.h"
#include "TileColor.h"

DefaultParseState::DefaultParseState(Parser& parser, const TSharedPtr<ParseState> parent) : ParseState(parser, parent){ }
DefaultParseState::~DefaultParseState() { }
void DefaultParseState::ParseAlpha(const char c)
{
	char x = 'r';
	
	if (parser.TileColorParseKey.Contains(c))
	{
		if (currentColor == ETileColor::None)
		{
			currentColor = parser.TileColorParseKey[c];
			CurrentTile()->SetColor(currentColor, true);
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