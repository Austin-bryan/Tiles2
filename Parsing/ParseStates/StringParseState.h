#pragma once
#include "CoreMinimal.h"
#include "ParseState.h"

/**
 * 
 */
class TILES2_API StringParseState final : public ParseState
{
public:
	explicit StringParseState(Parser& parser, TSharedPtr<ParseState> parent);
	~StringParseState();

	void ParseAlpha(const char c) override;
	void ParseQuotation() override;
	void ParseRightParen() override;
	FString Name() const override;
private:
	FString parsedText = "";
	bool inQuotes = false;
};