#pragma once
#include "CoreMinimal.h"
#include "ParseState.h"

struct FParameter;
/**
 * Base class for ParseStates that require parameters to be parsed in a state above, and returned to them.
 * Modules require parameter information from the ParameterState, and complex parameters such as FCoord
 * chain this, returning numbers to the coord state, to be returned to the module.
 */
class TILES2_API ParameterRequesterParseState : public ParseState
{
public:
	explicit ParameterRequesterParseState(Parser& parser, const TSharedPtr<ParseState> parent);
	~ParameterRequesterParseState();
	void AddParameter(FParameter&& parameter);
	virtual void OnParametersFinished(){}
protected:
	TArray<FParameter> parsedParameters;
};
