#include "Enums.h"
#include "Parser.h"
#include "Token.h"
#include "TileModules/ModuleFactory.h"
#include "Parsing/ParseStates/ModuleParseState.h"

#include "ParameterParseState.h"

ModuleParseState::ModuleParseState(Parser& parser, TSharedPtr<ParseState> parent) : ParseState(parser, parent) { isInDelimiter = false; }

void ModuleParseState::ParseAlpha(char c) { parsedText += fstr(c); }
void ModuleParseState::ParseDelimiter()
{
	FinishModuleState(TRightBrace, false);
}
void ModuleParseState::ParseRightBrace()
{
	FinishModuleState(TRightBrace, true);
}
void ModuleParseState::FinishModuleState(const char c, bool shouldPop)
{
	if (parsedText == fstr(""))
		Throw(TDelimiter);
	else if (!IsModuleValid())
		parser.Throw(parsedText, "Invalid module.");
	else
	{
		if (HasParameters())
		{
			if (!hasFinishedParameters)
				parser.Throw(TRightBrace, parsedText + fstr(" requires a parameter"));
		}
		else ModuleFactory::Produce(TileModuleParseKey[parsedText], CurrentTile());

		if (shouldPop)
			PopState();
	}
}
void ModuleParseState::ParseLeftParen()
{
	if (!HasParameters())
		 parser.Throw(TLeftParen, "Expected: '}'");
	else
	{
		hasFinishedParameters = true;
		PushState(EBoardParseState::Parameter);

		if (const auto parameterKey = ModuleParameterKey.Find(parsedText))
			StaticCastSharedPtr<ParameterParseState>(CurrentState())->ParseExpectedTypes(*parameterKey);
	}
}

FString ModuleParseState::GetExpectedMessage() { return "Expected a module."; }

bool ModuleParseState::HasParameters() const
{
	//Log(parsedText + SPC + ModuleParameterKey[parsedText]);
	return IsModuleValid() && ModuleParameterKey[parsedText] != "void"; 
}
bool ModuleParseState::IsModuleValid() const
{
	if (ModuleParameterKey.Contains(parsedText))
		return true;
	parser.Throw(parsedText, "Invalid module identifier");
	return false;
}
