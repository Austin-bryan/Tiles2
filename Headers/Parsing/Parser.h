#pragma once
#include "CoreMinimal.h"
#include "ForwardDeclares.h"
#include "TileModule.h"

class ATileSide;
enum class EBoardShape;
class ACreatorTile;
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
	void Parse(EBoardShape& shape, FCoordPtr& size, TilesMap& tilesMap);

	ParameterKey& GetParameterKey() const { return parameterKey; }
	EBoardShape BoardShape() const { return boardShape; }
	
	static TMap<char, ETileColor> TileColorParseKey;
	static TMap<ETileColor, char> ReverseTileColorParseKey;

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

public:
	static FString ReverseParseTile(const ACreatorTile* creatorTile);
private:
	static FString ReverseParseSide(const ACreatorTile* creatorTile, const ATileSide* side);
	static FString ReverseParseColor(ETileColor tileColor);
	static FString ReverseParseModules(TArray<ATileModule*> modules);

	bool shouldBreak = false;
};