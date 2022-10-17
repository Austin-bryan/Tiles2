#include "Parsing/ParseStates/ParseStateStack.h"
#include "DefaultParseState.h"
#include "ModuleParseState.h"
#include "ParameterParseState.h"
#include "NumberParseState.h"
#include "StringParseState.h"

ParseStateStack::ParseStateStack(Parser& parser) : parser{ parser }
{
	PushState(EBoardParseState::Default);
}
ParseStateStack::~ParseStateStack() { }

void ParseStateStack::PopState()
{
	CurrentStates.Pop();
}
void ParseStateStack::Print() const
{
	// for (const auto state : CurrentStates)
	// 	Log(state->Name(), FColor::Emerald, 0.0f);
}
void ParseStateStack::PushState(const EBoardParseState state)
{
	CurrentStates.Push(CreateState(state));
}
const TSharedPtr<ParseState>& ParseStateStack::CurrentState() const
{
	return CurrentStates[CurrentStates.Num() - 1];
}

TSharedPtr<ParseState> ParseStateStack::CreateState(const EBoardParseState state) const
{
	switch (state)
	{
	case EBoardParseState::Default:   return MakeShared<DefaultParseState>  (parser, nullptr);
	case EBoardParseState::Module:    return MakeShared<ModuleParseState>   (parser, CurrentState());
	case EBoardParseState::Parameter: return MakeShared<ParameterParseState>(parser, CurrentState());
	case EBoardParseState::Number:    return MakeShared<NumberParseState>   (parser, CurrentState());
	case EBoardParseState::String:    return MakeShared<StringParseState>   (parser, CurrentState());
	default: throw std::invalid_argument("Invalid parse state");
	}
}
