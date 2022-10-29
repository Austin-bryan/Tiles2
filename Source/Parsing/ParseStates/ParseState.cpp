#include "ParseStates/ParseState.h"
#include "Logger.h"
#include "Tile.h"
#include "ParseStateStack.h"
#include "Parser.h"

ParseState::ParseState(Parser& parser, const TSharedPtr<ParseState> parentState) : parser{ parser }, parent{ parentState }
{
	tokenFunctionMap = std::map<char, parseMethod>
	{
		{ TLeftBrace, &ParseState::ParseLeftBrace }, { TRightBrace, &ParseState::ParseRightBrace },
		{ TLeftParen, &ParseState::ParseLeftParen }, { TRightParen, &ParseState::ParseRightParen },
		{ TLeftAngle, &ParseState::ParseLeftAngle }, { TRightAngle, &ParseState::ParseRightAngle },
		{ TSeparator, &ParseState::ParseSeparator }, { TDivider,    &ParseState::ParseSeparator  },
		{ TPeriod,    &ParseState::ParsePeriod    }, { TDelimiter,  &ParseState::ParseDelimiter  },
		{ TSemiColon, &ParseState::ParseSemiColon }, { TQuotation,  &ParseState::ParseQuotation  },
		{ TNegation,  &ParseState::ParseNegation  }
	};
}
const TSharedPtr<ParseState>& ParseState::CurrentState() const
{
	return parser.stack->CurrentState();
}

ATile* ParseState::CurrentTile() const { return parser.CurrentTile; }
FString ParseState::GetExpectedMessage() { return fstr("Error: Unexpected character."); }

void ParseState::PopState() const
{
	parser.stack->CurrentState()->OnPopped();
	parser.stack->PopState();
}
void ParseState::PushState(const EBoardParseState state) const
{
	parser.stack->PushState(state);
	parser.stack->CurrentState()->OnPushed();
}
void ParseState::Throw(const char token) 
{ 
	parser.Throw(fstr(token), GetExpectedMessage());
}	
void ParseState::ParseChar(const char c, FString& parsedText)
{
	const char token = isalpha(c) ? TAlpha : isdigit(c) ? TDigit : c;

	if (isalpha(c))
		 ParseAlpha(c);
	else if (isdigit(c))
		 ParseNumber(c - '0');
	else (this->*tokenFunctionMap[token])();
}