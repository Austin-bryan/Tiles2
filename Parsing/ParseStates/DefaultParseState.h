#pragma once
#include "CoreMinimal.h"
#include "ParseState.h"
#include "Enums.h"

/**
 * 
 */
class TILES2_API DefaultParseState : public ParseState
{
public:
	explicit DefaultParseState(Parser& parser, TSharedPtr<ParseState> parent);
	~DefaultParseState();
	
	void ParseAlpha(const char c) override;
	void ParseSeparator() override;
	void ParseLeftBrace() override;
	void ParseSemiColon() override;
	FString Name() const override;
	void Reset();

	FString GetExpectedMessage() override;
private:
	ETileColor currentColor = ETileColor::None;
};
