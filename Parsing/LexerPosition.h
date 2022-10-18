// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"

/**
 * Maintains position for lexer, as well as column and tile count used later for debugging
 */
class TILES2_API LexerPosition
{
public:
	explicit LexerPosition(FString seed);
	void Next();
	int GetIndex() const { return index; }
	int GetTile() const { return tile; }
	void Reset();
private:
	int index = 0, tile = 1;
	FString seed;
};
