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

FString ParameterParseState::Name() const
{
	return "Parameter State";
}
void ParameterParseState::ParseExpectedTypes(const FString& parameterSeed, const bool waitForLeftParen)
{
	// const TSharedPtr<ModuleParseState> moduleState = StaticCastSharedPtr<ModuleParseState>(parent);
	// FString parameterSeed = ModuleParameterKey[moduleState->ParsedModule()];
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
			 if (parameter == INumber) expectedParameters.Add(&typeid(int));
		else if (parameter == IString) expectedParameters.Add(&typeid(FString));
		else if (parameter == ICoord)
			expectedParameters.Add(
				parser.BoardShape() == EBoardShape::Square
				? &typeid(FSquareCoord*)
				: parser.BoardShape() == EBoardShape::Triangle
				? &typeid(FTriCoord*)
				: &typeid(FHexCoord*));
	}
	Log(fstr("Seed: ") + SPC + seed);
	
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
	if (*expectedParameters[parameterIndex] == typeid(int))
		PushState(EBoardParseState::Number);
	else if (*expectedParameters[parameterIndex] == typeid(FString))
		PushState(EBoardParseState::String);
	/*TODO:: switch to enum or string here. Using typeid has no purpose
	TODO     and is preventing this from being extracted into function */
	else if (*expectedParameters[parameterIndex] == typeid(FSquareCoord*))
	{
		PushState(EBoardParseState::Parameter);
		
		const auto param = StaticCastSharedPtr<ParameterParseState>(CurrentState());
		param->ParseExpectedTypes(ModuleParameterKey[ISqrCoord], true);
		param->parameterType = &typeid(FSquareCoord*);
	}
	else if (*expectedParameters[parameterIndex] == typeid(FTriCoord*))
	{
		PushState(EBoardParseState::Parameter);
		
		const auto param = StaticCastSharedPtr<ParameterParseState>(CurrentState());
		param->ParseExpectedTypes(ModuleParameterKey[ITriCoord], true);
		param->parameterType = &typeid(FTriCoord*);
	}
	else if (*expectedParameters[parameterIndex] == typeid(FHexCoord*))
	{
		PushState(EBoardParseState::Parameter);
		
		const auto param = StaticCastSharedPtr<ParameterParseState>(CurrentState());
		param->ParseExpectedTypes(ModuleParameterKey[IHexCoord], true);
		param->parameterType = &typeid(FHexCoord*);
	}
	parameterIndex++;
}
void ParameterParseState::OnPushed() {}
void ParameterParseState::ParseLeftParen() { PushNextState(); }
void ParameterParseState::OnPopped()
{
	for (auto& p : parsedParameters)
		Log(p, FColor::Green);

	// If this is a non primative type
	if (parameterType.has_value())
	{
		if (**parameterType == typeid(FSquareCoord*) ||
			**parameterType == typeid(FHexCoord*) ||
			**parameterType == typeid(FTriCoord*))
			ConstructParameter<FCoord*>();
	}
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

	for (const auto& p : parsedParameters)
		Log(p.ToString(), FColor::Green);
	StaticCastSharedPtr<ParameterParseState>(parent)->AddParameter(std::move(coord));
}
void ParameterParseState::ParseDelimiter()
{
	PushNextState();
}
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
void ParameterParseState::AddParameter(FParameter&& parameter)
{
	for (auto& p : parsedParameters)
		Log(p, FColor::Blue);
	Log(parameter.ToString());
	parsedParameters.Add(parameter);
}
FString ParameterParseState::GetExpectedMessage()
{
	return "Invalid parameter syntax";
}
//189