#pragma once
#include "CoreMinimal.h"
#include "ParseState.h"

/**
 * Only parses floats, even if no decimals are parsed.
 */
class TILES2_API NumberParseState final : public ParseState
{
public:
	explicit NumberParseState(Parser& parser, const TSharedPtr<ParseState> parent);
	~NumberParseState();

	void ParseNumber(const int n) override;
	void ParseNegation() override;
	void ParsePeriod() override;
	void ParseDelimiter() override;
	void ExitNumberState() const;
	void ParseRightParen() override;
	FString Name() const override;
	float RoundToPlaces(float f, int decimalCount) const;

	FString GetExpectedMessage() override;
private:
	bool isFloat = false;
	int sign = 0;
	int parsedIntegers = 0;
	int parsedDecimals = 0;
};
