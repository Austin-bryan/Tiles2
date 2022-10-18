#pragma once
#include "CoreMinimal.h"
#include <map>
#include "Parsing/Token.h"

class Parser;
class ATile;
/**
 * 
 */
class TILES2_API ParseState
{
	using parseMethod = void(ParseState::*)(void);
	friend class Parser;
public:
	explicit ParseState(Parser& parser, const TSharedPtr<ParseState> parentState);
	virtual ~ParseState(){}
	
	virtual FString Name() const { return "Unused Parse State"; }
	const TSharedPtr<ParseState>& CurrentState() const;
protected:
	std::map<char, parseMethod> tokenFunctionMap;
	Parser& parser;
	const TSharedPtr<ParseState> parent;

	void PushState(EBoardParseState state) const;
	void PopState() const;

	ATile* CurrentTile() const;
	virtual FString GetExpectedMessage();
	virtual void OnPushed(){}
	virtual void OnPopped(){}
	void Throw(char token);

	virtual void ParseChar(const char c, FString& parsedText);
	virtual void ParseString(const FString& parsedText) { }
	virtual void ParseNumber(const int n) { Throw(TDigit); }
	virtual void ParseAlpha(const char c) { Throw(TAlpha); }
	virtual void ParseSeparator()  { Throw(TSeparator); }
	virtual void ParseSemiColon()  { Throw(TSemiColon); }
	virtual void ParseLeftBrace()  { Throw(TLeftBrace); }
	virtual void ParseLeftParen()  { Throw(TLeftParen); }
	virtual void ParseLeftAngle()  { Throw(TLeftAngle); }
	virtual void ParseRightBrace() { Throw(TRightBrace); }
	virtual void ParseRightParen() { Throw(TRightParen); }
	virtual void ParseRightAngle() { Throw(TRightAngle); }
	virtual void ParsePeriod()	   { Throw(TPeriod); }
	virtual void ParseDelimiter()  { Throw(TDelimiter); }
	virtual void ParseQuotation()  { Throw(TQuotation); }
	virtual void ParseNegation()   { Throw(TNegation); }
};