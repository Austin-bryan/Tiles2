#include "Enums.h"
#include "Parser.h"
#include "Token.h"
#include "TileModules/ModuleFactory.h"
#include "Parsing/ParseStates/ModuleParseState.h"

#include "ParameterParseState.h"

ModuleParseState::ModuleParseState(Parser& parser, const TSharedPtr<ParseState> parent) : ParameterRequesterParseState(parser, parent) { isInDelimiter = false; }

void ModuleParseState::ParseAlpha(char c) { parsedText += fstr(c); }
void ModuleParseState::ParseDelimiter()  { FinishModuleState(TRightBrace, false); }
void ModuleParseState::ParseRightBrace() { FinishModuleState(TRightBrace, true); }
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
void ModuleParseState::OnParametersFinished()
{
	for (const auto& parameter : parsedParameters)
		Log(fstr("Module Param: ") + parameter.ToString(), FColor::Red);
	ModuleFactory::Produce(TileModuleParseKey[parsedText], CurrentTile(), parsedParameters);
}

FString ModuleParseState::GetExpectedMessage() { return "Expected a module."; }

bool ModuleParseState::HasParameters() const
{
	return IsModuleValid() && ModuleParameterKey[parsedText] != "void"; 
}
bool ModuleParseState::IsModuleValid() const
{
	if (ModuleParameterKey.Contains(parsedText))
		return true;
	parser.Throw(parsedText, "Invalid module identifier");
	return false;
}
