#include "ParameterParseState.h"
#include "ModuleParseState.h"
#include "CoordHeaders.h"
#include "Token.h"
#include "Parser.h"
#include "Logger.h"
#include "ParameterKey.h"

FParameter::FParameter(const float f)	 	 { variant.Set<float>(f); }
FParameter::FParameter(const char* string)   { variant.Set<FString>(string); }
FParameter::FParameter(const FString string) { variant.Set<FString>(string); }
FParameter::FParameter(const FCoordPtr coord){ variant.Set<FCoordPtr>(coord); }

template <typename T>
const T& FParameter::Get() const { return variant.Get<T>(); }

template <typename T>
const T* FParameter::GetIf() const { return variant.TryGet<T>(); }

FString FParameter::ToString() const
{
	if (const float* number = GetIf<float>())
		return fstr(*number);
	if (const auto& string = GetIf<FString>())
		return *string;
	if (const auto coord = GetIf<FCoordPtr>())
		return (*coord)->ToString();
	return "Invalid type.";
}

// ---------------- ParameterParseState ---------------- //
ParameterParseState::ParameterParseState(Parser& parser, const TSharedPtr<ParseState> parent) : ParameterRequesterParseState{parser, parent} { }

FString ParameterParseState::Name() const { return "Parameter State"; }
void ParameterParseState::ParseExpectedTypes(const FString& parameterSeed, const bool waitForLeftParen)
{
	const auto seed = parameterSeed.Replace(*fstr(" "), *fstr(""));

	TArray<FString> parameters;
	seed.ParseIntoArray(parameters, *fstr(","));

	for (const auto& parameter : parameters)
	{
			 if (parameter == INumber) expectedParameters.Add(EParameter::Number);
		else if (parameter == IString) expectedParameters.Add(EParameter::String);
		else if (parameter == ICoord)  expectedParameters.Add(EParameter::Coord);
		else if (parameter == IVoid)
		{
			parser.Throw(parameter, "IVoid is not a parameter type.");
			break;
		}
	}
	if (!waitForLeftParen)
		PushNextState();
}
void ParameterParseState::PushNextState()
{
	if (parameterIndex >= expectedParameters.Num())
	{
		parser.Throw("Error: ", "Too many parameters given.");
		return;
	}
	switch (expectedParameters[parameterIndex])
	{
	case EParameter::Number: PushState(EBoardParseState::Number); break;
	case EParameter::String: PushState(EBoardParseState::String); break;
	case EParameter::Coord:  AddCoordParameter(EParameter::Coord, ICoord);  break;
	default: Log("invalid expected parameter state.");
	}
	parameterIndex++;
}
void ParameterParseState::AddCoordParameter(EParameter childCoord, const FString& coordKey) const
{
	PushState(EBoardParseState::Parameter);

	const auto param = StaticCastSharedPtr<ParameterParseState>(CurrentState());
	param->ParseExpectedTypes(parser.GetParameterKey()[coordKey], true);
	param->parameterType = childCoord;
}

void ParameterParseState::OnPushed() {}
void ParameterParseState::ParseLeftParen() { PushNextState(); }
void ParameterParseState::OnPopped()
{
	// Called if parameter is a complex/non-primative type
	if (parameterType.has_value())
		if (parameterType == EParameter::Coord)
			ConstructParameter<FCoordPtr>();
	const auto requester = StaticCastSharedPtr<ParameterRequesterParseState>(parent);
	for (auto& parameter : parsedParameters)
		requester->AddParameter(std::move(parameter));
	requester->OnParametersFinished();
}

template <typename T>
void ParameterParseState::ConstructParameter(){}

template <>
void ParameterParseState::ConstructParameter<FCoordPtr>()
{
	/*
	 * FCoord* is a composite type, so this is a nested paramater.
	 * After all numbers have been parsed, this creates coord
	 */
	auto coordMember = [this](const int index) { return parsedParameters[index].Get<float>(); };

	const FCoordPtr coord =
 		  parser.BoardShape() == EBoardShape::Square
 		? static_cast<FCoordPtr>(MakeShared<FSqrCoord>(coordMember(0), coordMember(1)))
 		: parser.BoardShape() == EBoardShape::Triangle
 		? static_cast<FCoordPtr>(MakeShared<FTriCoord>(coordMember(0), coordMember(1), coordMember(2), true))
 		: static_cast<FCoordPtr>(MakeShared<FHexCoord>(coordMember(0), coordMember(1), coordMember(2)));

	parsedParameters.Empty();
	parsedParameters.Add(FParameter(coord));
}

void ParameterParseState::ParseDelimiter() { PushNextState(); }
void ParameterParseState::ParseRightParen()
{
	if (parameterIndex < expectedParameters.Num())
	{
		parser.Throw(TRightParen,
			StaticCastSharedPtr<ModuleParseState>(parent)->ParsedModule()
			+ " requires "_f + fstr(expectedParameters.Num()) + " parameters"_f);
		return;
	}
	PopState();
}
FString ParameterParseState::GetExpectedMessage()
{
	return "Invalid parameter syntax";
}
//189