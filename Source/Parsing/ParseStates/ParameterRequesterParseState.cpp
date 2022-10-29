#include "ParseStates/ParameterRequesterParseState.h"

ParameterRequesterParseState::ParameterRequesterParseState(Parser& parser, const TSharedPtr<ParseState> parent)
    : ParseState{ parser, parent } { }
ParameterRequesterParseState::~ParameterRequesterParseState() { }

void ParameterRequesterParseState::AddParameter(FParameter&& parameter) { parsedParameters.Add(parameter); }