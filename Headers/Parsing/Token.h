#pragma once
#include "CoreMinimal.h"
#include "Enums.h"

/*
	Board Modes:
	Sqr, Tri, Hex
	Loop (hex based)
	Line (sqr based)
	Gear (sqr based) Swiping a layer automatically swipes adjanct layers in opposite direction
*/	

constexpr char TLeftBrace   = '{', TRightBrace = '}';
constexpr char TLeftParen   = '(', TRightParen = ')';
constexpr char TLeftAngle   = '<', TRightAngle = '>';
constexpr char TSeparator   = '/', TDivider    = '|';
constexpr char TPeriod      = '.', TDelimiter  = ',';
constexpr char TAlpha       = 'A', TDigit      = 'D';
constexpr char TSemiColon   = ';', TQuotation  = '\"';
constexpr char TNegation	= '-';

const FString INormal   = "",  IRift = "rft", IWrap = "w",   ICircuitWrap = "cir", IBandaged       = "bnd", ICloud = "cld";
const FString INoSpawn  = "#", ILink = "lnk", ISwap = "swp", IRotator	  = "rot", ICorrectCounter = "cnt";
const FString IGap      = "x", IIron = "irn", ICamo = "cmo", IMirrorWrap  = "mir", ITeleportWrap   = "tel";
const FString IVoid     = "void", IString = "string", INumber = "number", ICoord = "coord";
const FString TSqrBoard = "sqr";
const FString THexBoard = "hex";
const FString TTriBoard = "tri";

// ReSharper disable once StringLiteralTypo
const FString Colors = "wroygcpnmk";
const FString Tokens = "{}()<>,.|/\"-;";

const auto TileModuleParseKey = TMap<FString, EModule>
{
	{ INormal,      EModule::Normal  },     { IRift,	       EModule::Rift },     { IWrap,  EModule::Wrap },
	{ ICircuitWrap, EModule::CircuitWrap }, { IBandaged,	   EModule::Bandaged }, { ICamo,  EModule::Camo },
	{ INoSpawn,     EModule::NoSpawn },     { ILink,		   EModule::Link },     { ISwap,  EModule::Swap },
	{ IGap,	        EModule::Gap },         { IIron,		   EModule::Iron },     { ICloud, EModule::Cloud },
	{ IRotator,	    EModule::Rotator },     { ICorrectCounter, EModule::CorrectCounter },
	{ IMirrorWrap,  EModule::MirrorWrap },  { ITeleportWrap,   EModule::TeleportWrap },
};

/*
 	Iron - Infnite number of swipes, but once swiped into correct position it nails itself down and becomes a wraptile
		Notes:
			this could behave poorly with teleport wrap tiles
		Syntax:
			irn
	Cloud - Once swiped into correct position, turns into gap tile
		Syntax:
			cld
	Swap -
		Syntax: 
			swp
	Rotator - 
		Syntax:
			rot(<cc | cw>)
	Camo -
		Syntax:
			cmo
	Wrap -
		Syntax: 
			z | z(tel | mir | cir)
	Bandaged - 
		Syntax:
			bnd((x0, y0), (x1, x2)) | bnd((x0, y0, z0), (x1, x1, z1)) 
			bnd<coord, coord>
		Notes:
			This requires support for multiple parameters ("ICoord, ICoord")
			Then Parameter state needs to be able to parse through each deliminated item in the string then differ to parseStates
				to parse the variable type. Build array of expected types, maintain index. Parse type, transfer state,
				then send back the return value and store that some how.

			Once all types are parsed, it creates module passing it along, a different function overload for each combination
				that points to a different factory method with a switch for that module combo
	Sub -
		Syntax:
			sub((x, y)) | bnd((x, y, z))
			sub<coord>
	Correct Counter -
		Syntax: 
			cnt(int)
*/