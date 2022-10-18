#include "ParameterParseState.h"
#include "ModuleParseState.h"
#include "Coord/Coord.h"
#include "Token.h"
#include "Parser.h"
#include "Logger.h"
#include "SquareCoord.h"
#include "TriCoord.h"
#include "HexCoord.h"

FParameter::FParameter(const float f)	 	 { variant.Set<float>(f); }
FParameter::FParameter(const char* string)   { variant.Set<FString>(string); }
FParameter::FParameter(const FString string) { variant.Set<FString>(string); }
FParameter::FParameter(FCoord* coord)		 { variant.Set<FCoord*>(coord); }

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
	if (const auto coord = GetIf<FCoord*>())
		return (*coord)->ToString();
	return "Invalid type.";
}

// ---------------- ParameterParseState ---------------- //
ParameterParseState::ParameterParseState(Parser& parser, TSharedPtr<ParseState> parent) : ParseState{parser, parent} { }

FString ParameterParseState::Name() const { return "Parameter State"; }
void ParameterParseState::ParseExpectedTypes(const FString& parameterSeed, const bool waitForLeftParen)
{
	const auto seed = parameterSeed.Replace(*fstr(" "), *fstr(""));

	TArray<FString> parameters;
	seed.ParseIntoArray(parameters, *fstr(","));
	
	for (const auto& parameter : parameters)
	{
		if (parameter == IVoid)
		{
			parser.Throw(parameter, "IVoid is not a parameter type.");
			break;
		}
			 if (parameter == INumber) expectedParameters.Add(EParameter::Number);
		else if (parameter == IString) expectedParameters.Add(EParameter::String);
		else if (parameter == ICoord)
			expectedParameters.Add(
				parser.BoardShape() == EBoardShape::Square
				? EParameter::SqrCoord
				: parser.BoardShape() == EBoardShape::Hex
				? EParameter::HexCoord
				: EParameter::TriCoord);
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
	if (expectedParameters[parameterIndex] == EParameter::Number)
		PushState(EBoardParseState::Number);
	else if (expectedParameters[parameterIndex] == EParameter::String)
		PushState(EBoardParseState::String);
	else if (expectedParameters[parameterIndex] == EParameter::SqrCoord)
		AddCoordParameter(EParameter::SqrCoord, ISqrCoord);
	else if (expectedParameters[parameterIndex] == EParameter::HexCoord)
		AddCoordParameter(EParameter::HexCoord, IHexCoord);
	else if (expectedParameters[parameterIndex] == EParameter::TriCoord)
		AddCoordParameter(EParameter::TriCoord, ITriCoord);
	parameterIndex++;
}
void ParameterParseState::AddCoordParameter(EParameter childCoord, const FString& coordKey) const
{
	PushState(EBoardParseState::Parameter);
	
	const auto param = StaticCastSharedPtr<ParameterParseState>(CurrentState());
	param->ParseExpectedTypes(ModuleParameterKey[coordKey], true);
	param->parameterType = childCoord;
}

void ParameterParseState::OnPushed() {}
void ParameterParseState::ParseLeftParen() { PushNextState(); }
void ParameterParseState::OnPopped()
{
	// Called if parameter is a complex/non-primative type
	if (parameterType.has_value())
	{
		if (parameterType == EParameter::SqrCoord ||
			parameterType == EParameter::HexCoord ||
			parameterType == EParameter::TriCoord)
			ConstructParameter<FCoord*>();
	}
	for (const auto& parameter : parsedParameters)
		Log(parameter.ToString(), FColor::Green);
}
template <typename T>
void ParameterParseState::ConstructParameter(){}

template <>
void ParameterParseState::ConstructParameter<FCoord*>()
{
	auto coordMember = [this](const int index) { return parsedParameters[index].Get<float>(); };
	FCoord* coord = parser.BoardShape() == EBoardShape::Square
		? new FSquareCoord(coordMember(0), coordMember(1))
		: parser.BoardShape() == EBoardShape::Triangle
		? static_cast<FCoord*>(new FTriCoord(coordMember(0), coordMember(1), coordMember(2), true))
		: new FHexCoord(coordMember(0), coordMember(1), coordMember(2));
	parsedParameters.Empty();
	parsedParameters.Add(coord);


	StaticCastSharedPtr<ParameterParseState>(parent)->AddParameter(std::move(coord));
}

void ParameterParseState::ParseDelimiter() { PushNextState(); }
void ParameterParseState::ParseRightParen()
{
	if (parameterIndex < expectedParameters.Num())
	{
		parser.Throw(TRightParen, StaticCastSharedPtr<ModuleParseState>(parent)->ParsedModule() + fstr(" requires ")
			+ fstr(expectedParameters.Num()) + " parameters");
		return;
	}
	PopState();
}
void ParameterParseState::AddParameter(FParameter&& parameter) { parsedParameters.Add(parameter); }
FString ParameterParseState::GetExpectedMessage()
{
	return "Invalid parameter syntax";
}
//189