#pragma once
#include "CoreMinimal.h"
#include "ParameterRequesterParseState.h"
#include "ParseState.h"

/**
 *
 */
class TILES2_API ModuleParseState : public ParameterRequesterParseState
{
	using onModuleFinished = void(ModuleParseState::*)(void);
public:
	explicit ModuleParseState(Parser& parser, const TSharedPtr<ParseState> parent);
	~ModuleParseState() override {}

	void ParseAlpha(char c) final;
	void ParseDelimiter() final;
	void ParseRightBrace() final;
	void FinishModuleState(const char c, const bool shouldPop);
	void ParseLeftParen() final;
	FString Name() const override { return "Module State"; }
	void OnParametersFinished() override;;
	
	const FString& ParsedModule() const { return parsedText; }
	FString GetExpectedMessage() final;
private:
	FString parsedText;
	bool isInDelimiter;
	bool hasFinishedParameters = false;
	
	bool IsModuleValid() const;
	bool HasParameters() const;
};
