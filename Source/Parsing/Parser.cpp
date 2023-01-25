#include "Parser.h"

#include <string>
#include "Board.h"
#include "BoardPopulator.h"
#include "BoardPopulatorFactory.h"
#include "Coord.h"
#include "CreatorTile.h"
#include "Enums.h"
#include "LexerPosition.h"
#include "Logger.h"
#include "ParseError.h"
#include "ParseState.h"
#include "ParseStateStack.h"
#include "Tile.h"
#include "Token.h"
#include "ParameterKey.h"
#include "TileColor.h"
#include "TileSide.h"

ParameterKey Parser::parameterKey;
TMap<char, ETileColor> Parser::TileColorParseKey;
TMap<ETileColor, char> Parser::ReverseTileColorParseKey;

ATile* Parser::CurrentTile;
Parser::Parser(ABoard* board, FString seed) : board{ board }, seed{ seed }
{
	currentChar = '\0';
	seed.RemoveSpacesInline();
	seed.Split(FString("|"), &boardSeed, &tileSeed);
	
	parseError = MakeUnique<ParseError>(tileSeed);
	pos        = MakeUnique<LexerPosition>(tileSeed);
	stack      = new ParseStateStack(*this);

	TileColorParseKey = TMap<char, ETileColor>
	{
		{ 'w', ETileColor::White },   { 'r', ETileColor::Red },    { 'o', ETileColor::Orange },
		{ 'y', ETileColor::Yellow },  { 'g', ETileColor::Green },  { 'c', ETileColor::Cyan },
		{ 'b', ETileColor::Blue },    { 'p', ETileColor::Purple }, { 'n', ETileColor::Pink },
		{ 't', ETileColor::Magenta }, { 'k', ETileColor::Black },  { 'm', ETileColor::Maroon }
	};
	for (auto tileColorParseKey : TileColorParseKey)
		ReverseTileColorParseKey.Add(tileColorParseKey.Value, tileColorParseKey.Key);
}

void Parser::Parse(EBoardShape& shape, FCoordPtr& size, TilesMap& tilesMap)
{
	FString parsedText;
	ATile::ResetTileCount();
	
	SetupBoard(shape, size);
	parameterKey = ParameterKey(shape);

	const auto populator = BoardPopulatorFactory::Create(board);
	populator->Populate(size, tilesMap);

	if (tilesMap.Num() < 1) { Log("no tiles"); return; }
	
	TArray<ATile*> spawnedTiles = tilesMap.Values();
	CurrentTile = spawnedTiles[0];

	for (int i = 0; i < tileSeed.Len(); i++)
	{
		if (shouldBreak)
			break;
		const int index = pos->GetTile() - 1;
		currentChar = tileSeed[i];

		if (index < 0 || index >= spawnedTiles.Num())
		{
			Throw(currentChar, fstr("Expected: ';'"));
			break;
		}
		CurrentTile = spawnedTiles[index];

		if (!isalnum(currentChar) && !Tokens.Contains(fstr(currentChar)))
		{
			Throw(currentChar, "Identifier or token expected"_f);
			break;
		}
		stack->CurrentState()->ParseChar(currentChar, parsedText);

		if (currentChar != TSemiColon && i + 1 < seed.Len())
			pos->Next();
	}
}
void Parser::SetupBoard(EBoardShape& shape, FCoordPtr& coord)
{
	// Sets board shape and coord if seed is correct
	const FString boardShapeSeed = boardSeed.Left(3);

	if (boardShapeSeed != TSqrBoard && boardShapeSeed != TTriBoard && boardShapeSeed != THexBoard)
	{
		Throw(boardShapeSeed, "Invalid board shape"_f);
		return;
	}
	shape = boardShapeSeed == TSqrBoard
		        ? EBoardShape::Square
		        : boardShapeSeed == TTriBoard
		        ? EBoardShape::Triangle
		        : EBoardShape::Hex;
	boardShape = shape;
	ParseBoardSize(coord, shape);
}
void Parser::ParseBoardSize(FCoordPtr& coord, const EBoardShape& shape)
{
	FString dump;
	FString coordSeed;
	boardSeed.Split(fstr("("), &dump, &coordSeed);
	char expectedToken     = TDigit;
	int coordMembers[3]    = { 0, 0, 0 };
	int currentCoordMember = 0;
	const int maxDigits    = shape == EBoardShape::Square ? 2 : 3;
	std::string numString  = "";

	for (int i = 0; i < coordSeed.Len(); i++)
	{
		pos->Next();
		const char c = coordSeed[i];

		// Keep concatinating number while parsing an unteruppted string of numbers
		if (isdigit(c) && expectedToken == TDigit)
			numString += c;
		// String of numbers is interupted then we haved finished parsing a number
		if (!isdigit(c) && expectedToken == TDigit && numString.length() > 0)
		{
			coordMembers[currentCoordMember] = stoi(numString);

			if (currentCoordMember < maxDigits - 1) // We're not done yet, try to parse next Coord member
				expectedToken = ',';
			else if (currentCoordMember == maxDigits - 1) // We are done now, there should be a ')' now
				expectedToken = ')';
			currentCoordMember++;
			numString = ""; // Reset numstring for next number
		}
		// Throw if c isnt the expected token. Digits are an exception as their token is 'D' so there is an extra clause for them
		if (c != expectedToken && !(isdigit(c) && expectedToken == TDigit))
		{
			const auto error = MakeUnique<ParseError>(coordSeed);
			Throw(fstr(c), fstr(expectedToken), error);
			break;
		}
		if (c == TDelimiter && expectedToken == TDelimiter) // Just parsed a ',' digit should be next
			expectedToken = TDigit;
		if (c == TRightParen && expectedToken == TRightParen) // We're finished here
			break;
	}
	coord = FCoord::Create(shape, coordMembers[0], coordMembers[1], coordMembers[2]);
	pos->Reset();
}

FString Parser::ReverseParseTile(const ACreatorTile* creatorTile)
{
	if (creatorTile->SideCount() == 1)
		return ReverseParseSide(creatorTile, creatorTile->CurrentSide());

	FString parsedText = "<";

	for (auto side : creatorTile->GetSides())
		parsedText += ReverseParseSide(creatorTile, side);
	return parsedText + ">";
}
FString Parser::ReverseParseColor(ETileColor tileColor)			  						 { return fstr(ReverseTileColorParseKey[tileColor]); }
FString Parser::ReverseParseModules(TArray<ATileModule*> modules) 						 { return ""; }
FString Parser::ReverseParseSide(const ACreatorTile* creatorTile, const ATileSide* side) { return ReverseParseColor(creatorTile->GetColor()); }

void Parser::Throw(char    error, FString&& expected) { Throw(error, std::move(expected), parseError); }
void Parser::Throw(FString error, FString&& expected) { Throw(error, std::move(expected), parseError); }
void Parser::Throw(char    error, FString&& expected, const TUniquePtr<ParseError>& errorThrower) { Throw(fstr(error), std::move(expected), errorThrower); }
void Parser::Throw(FString error, FString&& expected, const TUniquePtr<ParseError>& errorThrower)
{
	errorThrower->Throw(pos, error, expected, stack->CurrentState()->Name());
	shouldBreak = true;
}