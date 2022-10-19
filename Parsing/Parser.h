#pragma once
#include "CoreMinimal.h"
#include "ParseError.h"
#include "LexerPosition.h"
#include "ForwardDeclares.h"

struct FCoord;
enum class EBoardShape;
class ABoard;
class ATile;
class LexerPosition;
class ParseError;
class ParseStateStack;
/**
 * Categorizes seed text into tokens
 */
class TILES2_API Parser
{
	friend class ParseState;
public:
	Parser(ABoard* board, FString seed);
	
	void Throw(const char error,    FString&& expected);
	void Throw(const char error,    FString&& expected, const TUniquePtr<ParseError>& errorThrower);
	void Throw(const FString error, FString&& expected);
	void Throw(const FString error, FString&& expected, const TUniquePtr<ParseError>& errorThrower);
	void Parse(EBoardShape& shape, FCoordPtr& coord, Tiles& tiles);

	EBoardShape BoardShape() const { return boardShape; }
protected:
	static ATile* CurrentTile;
private:
	ABoard* board;
	FString boardSeed, seed, tileSeed;
	char currentChar;
	TUniquePtr<LexerPosition> pos;
	ParseStateStack* stack;
	EBoardShape boardShape;

	TUniquePtr<ParseError> parseError;
	void SetupBoard(EBoardShape& shape, FCoordPtr& coord);
	void ParseBoardSize(FCoordPtr& coord, const EBoardShape& shape);

	bool shouldBreak = false;
};