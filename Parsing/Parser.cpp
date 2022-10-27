#include "Parser.h"

#include <string>
#include "Board.h"
#include "BoardPopulator.h"
#include "BoardPopulatorFactory.h"
#include "Coord.h"
#include "Enums.h"
#include "LexerPosition.h"
#include "Logger.h"
#include "ParseError.h"
#include "ParseState.h"
#include "ParseStateStack.h"
#include "Tile.h"
#include "Token.h"

ATile* Parser::CurrentTile;
Parser::Parser(ABoard* board, FString seed) : board{ board }, seed{ seed }
{
	currentChar = '\0';
	seed.RemoveSpacesInline();
	seed.Split(FString("|"), &boardSeed, &tileSeed);
	
	parseError = MakeUnique<ParseError>(tileSeed);
	pos        = MakeUnique<LexerPosition>(tileSeed);
	stack      = new ParseStateStack(*this);
}

void Parser::Parse(EBoardShape& shape, FCoordPtr& size, Tiles& tiles)
{
	FString parsedText;
	ATile::ResetTileCount();
	
	SetupBoard(shape, size);

	if (size == nullptr)
	{
		Log("Null coord @ Parser.cpp line 29");
		return;
	}
	const auto populator = BoardPopulatorFactory::Create(board);
	populator->Populate(size, tiles);

	TArray<ATile*> spawnedTiles = tiles.Values();
	// for (const auto& [first, second] : tiles)
	// 	spawnedTiles.Add(second);
	// spawnedTiles.Sort([](const ATile& a, const ATile& b) -> bool
	// {
	// 	return a.ID() < b.ID();
	// });
	//
	if (spawnedTiles.Num() < 1)
	{
		Log("no tiles");
		return;
	}
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
			Throw(currentChar, fstr("Identifier or token expected"));
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
		Throw(boardShapeSeed, fstr("Invalid board shape"));
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

void Parser::Throw(const char    error, FString&& expected)
{
	Throw(error, std::move(expected), parseError);
}
void Parser::Throw(const FString error, FString&& expected)
{
	Throw(error, std::move(expected), parseError);
}
void Parser::Throw(const char    error, FString&& expected, const TUniquePtr<ParseError>& errorThrower)
{
	Throw(fstr(error), std::move(expected), errorThrower);
}
void Parser::Throw(const FString error, FString&& expected, const TUniquePtr<ParseError>& errorThrower)
{
	errorThrower->Throw(pos, error, expected, stack->CurrentState()->Name());
	shouldBreak = true;
}