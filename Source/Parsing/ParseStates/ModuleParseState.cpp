#include "Logger.h"
#include "Parser.h"
#include "Token.h"
#include "ModTile.h"
#include "ModuleFactory.h"
#include "ParameterKey.h"
#include "ParameterParseState.h"
#include "ParseStates/ModuleParseState.h"

ModuleParseState::ModuleParseState(Parser& parser, const TSharedPtr<ParseState> parent) : ParameterRequesterParseState(parser, parent) { isInDelimiter = false; }

void ModuleParseState::ParseAlpha(char c) { parsedText += fstr(c); }
void ModuleParseState::ParseDelimiter()   { FinishModuleState(TRightBrace, false); }
void ModuleParseState::ParseRightBrace()  { FinishModuleState(TRightBrace, true); }
void ModuleParseState::FinishModuleState(const char c, const bool shouldPop)
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
		else ModuleFactory::Produce(TileModuleParseKey[parsedText], static_cast<AModTile*>(CurrentTile()));

		if (shouldPop)
			 PopState();
		else parsedText = "";
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

		if (const auto parameterKey = parser.GetParameterKey().Find(parsedText))
			StaticCastSharedPtr<ParameterParseState>(CurrentState())->ParseExpectedTypes(*parameterKey);
	}
}
void ModuleParseState::OnParametersFinished()
{
	ModuleFactory::Produce(TileModuleParseKey[parsedText], static_cast<AModTile*>(CurrentTile()), parsedParameters);
}
FString ModuleParseState::GetExpectedMessage() { return "Expected a module."; }

bool ModuleParseState::HasParameters() const
{
	return IsModuleValid() && parser.GetParameterKey()[parsedText] != "void"; 
}
bool ModuleParseState::IsModuleValid() const
{
	if (parser.GetParameterKey().Contains(parsedText))
		return true;
	parser.Throw(parsedText, "Invalid module identifier");
	return false;
}