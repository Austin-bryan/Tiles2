#pragma once
#include "CoreMinimal.h"
#include "Enums.h"

class Parser;
class ParseState;
class DefaultParseState;
class Default;
/**
 * 
 */
class TILES2_API ParseStateStack
{
public:
	explicit ParseStateStack(Parser& parser);
	~ParseStateStack();

	const TSharedPtr<ParseState>& CurrentState() const;
	TSharedPtr<ParseState> CreateState(const EBoardParseState state) const;

	void PushState(EBoardParseState state);
	void PrintStack(const FColor color) const;
	void PopState();
private:
	TArray<TSharedPtr<ParseState>> CurrentStates;
	Parser& parser;
};