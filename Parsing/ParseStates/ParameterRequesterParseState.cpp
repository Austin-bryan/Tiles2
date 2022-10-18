#include "Parsing/ParseStates/ParameterRequesterParseState.h"

ParameterRequesterParseState::ParameterRequesterParseState(Parser& parser, TSharedPtr<ParseState> parent)
    : ParseState{ parser, parent } { }
ParameterRequesterParseState::~ParameterRequesterParseState() { }

void ParameterRequesterParseState::AddParameter(FParameter&& parameter) { parsedParameters.Add(parameter); }