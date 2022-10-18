#pragma once
#include "CoreMinimal.h"
#include "ParseState.h"

/**
 * 
 */
class TILES2_API ParameterRequesterParseState : public ParseState
{
public:
	explicit ParameterRequesterParseState(Parser& parser, TSharedPtr<ParseState> parent);
	~ParameterRequesterParseState();
	void AddParameter(FParameter&& parameter);
	virtual void OnParametersFinished(){}
protected:
	TArray<FParameter> parsedParameters;
};
