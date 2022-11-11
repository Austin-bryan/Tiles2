#pragma once
#include "CoreMinimal.h"
#include "ParseState.h"
#include "TileColor.h"

/**
 * The base fallback state. Used to detect colors and seperation of tiles.
 */
class TILES2_API DefaultParseState : public ParseState
{
public:
	explicit DefaultParseState(Parser& parser, const TSharedPtr<ParseState> parent);
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
