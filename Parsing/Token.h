#pragma once
#include <initializer_list>
#include "CoreMinimal.h"
#include "Enums.h"
#include "Logger.h"

/*
	Board Modes:
	Sqr, Tri, Hex
	Loop (hex based)
	Line (sqr based)
	Gear (sqr based) Swiping a layer automatically swipes adjanct layers in opposite direction
*/	

FString format(const std::initializer_list<FString> args);

constexpr char TLeftBrace   = '{', TRightBrace = '}';
constexpr char TLeftParen   = '(', TRightParen = ')';
constexpr char TLeftAngle   = '<', TRightAngle = '>';
constexpr char TSeparator   = '/', TDivider    = '|';
constexpr char TPeriod      = '.', TDelimiter  = ',';
constexpr char TAlpha       = 'A', TDigit      = 'D';
constexpr char TSemiColon   = ';', TQuotation  = '\"';
constexpr char TNegation	= '-';

const FString INormal   = "",  IRift ="rft", IWrap = "z",   ICircuitWrap = "cir", IBandaged       = "bnd", ICloud = "cld";
const FString INoSpawn  = "#", ILink ="lnk", ISwap = "swp", IRotator	 = "rot", ICorrectCounter = "cnt";
const FString IGap      = "x", IIron ="irn", ICamo = "cmo", IMirrorWrap  = "mir", ITeleportWrap   = "tel";
const FString IVoid     = "void", IString = "string", INumber = "number", ICoord = "coord";
const FString ISqrCoord = "sqrCoord";
const FString IHexCoord = "hexCoord";
const FString ITriCoord = "triCoord";
const FString TSqrBoard = "sqr";
const FString THexBoard = "hex";
const FString TTriBoard = "tri";

// ReSharper disable once StringLiteralTypo
const FString Colors	= "wroygcpnmk";
const FString Tokens	= "{}()<>,.|/\"-;";
const TMap<char, ETileColor>  TileColorParseKey = TMap<char, ETileColor>
{
	{ 'w', ETileColor::White },   { 'r', ETileColor::Red },   { 'o', ETileColor::Orange },
	{ 'y', ETileColor::Yellow },  { 'g', ETileColor::Green }, { 'c', ETileColor::Cyan },
	{ 'b', ETileColor::Blue },    { 'p', ETileColor::Purple },{ 'n', ETileColor::Pink },
	{ 'm', ETileColor::Magenta }, { 'k', ETileColor::Black }
};
const TMap<FString, EModule> TileModuleParseKey = TMap<FString, EModule>
{
	{ INormal,      EModule::Normal  },     { IRift,	       EModule::Rift },     { IWrap,  EModule::Wrap },
	{ ICircuitWrap, EModule::CircuitWrap }, { IBandaged,	   EModule::Bandaged }, { ICamo,  EModule::Camo },
	{ INoSpawn,     EModule::NoSpawn },     { ILink,		   EModule::Link },     { ISwap,  EModule::Swap },
	{ IGap,	       EModule::Gap },         { IIron,		   EModule::Iron },     { ICloud, EModule::Cloud },
	{ IRotator,	   EModule::Rotator },     { ICorrectCounter, EModule::CorrectCounter },
	{ IMirrorWrap,  EModule::MirrorWrap },  { ITeleportWrap,   EModule::TeleportWrap },
};
const TMap<FString, FString> ModuleParameterKey = TMap<FString, FString>
{
		{ INormal,  IVoid }, { IRift, IVoid }, { IWrap,  IVoid },
		{ INoSpawn, IVoid }, { ILink, IVoid }, { ISwap,  IVoid },
		{ IGap,	   IVoid }, { IIron, IVoid }, { ICloud, IVoid },
	    { ICamo,    IVoid },
		{ ICircuitWrap, IVoid },
		{ IRotator,	   IString }, { ICorrectCounter,format({INumber, INumber}) },
		{ IMirrorWrap,  IVoid },   { ITeleportWrap,  format({INumber, IString, INumber, INumber})},
        { IBandaged, format({INumber, ICoord, IString}) },
		{ ISqrCoord, format({INumber, INumber}) },
		{ ITriCoord, format({INumber, INumber, INumber}) },
		{ IHexCoord, format({INumber, INumber, INumber}) },
};
inline FString format(const std::initializer_list<FString> args)
{
	FString result;
	
	int i = 0;
	for (auto& s : args)
	{
		result += s;
		if (i < args.size() - 1)
			result += fstr(", ");
		i++;
	}
	return result;
}
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


/**
 * 
 */
class TILES2_API Token
{
public:
	Token(char type, char value = '\0');

	char GetType()  const { return type; }
	char GetValue() const { return value; };

	static bool CreateToken(char c, Token& result);
	FString ToString() const;
private:
	char type, value;
};
