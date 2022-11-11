#pragma once
#include "CoreMinimal.h"
#include "ForwardDeclares.h"
#include "TilesMap.h"
#include "Coord.h"

enum class EBoardShape;
class ABoard;
class ATile;
class LexerPosition;
class ParseError;
class ParseStateStack;
class ParameterKey;

/**
 * Parses seed texts into instructions for creating tiles.
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
	void Parse(EBoardShape& shape, FCoordPtr& size, Tiles& tiles);

	ParameterKey& GetParameterKey() const { return parameterKey; }
	EBoardShape BoardShape() const { return boardShape; }
	TMap<char, ETileColor> TileColorParseKey;
	
protected:
	static ATile* CurrentTile;
private:
	static ParameterKey parameterKey;
	
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