#pragma once
#include "ParameterKey.h"
#include "Token.h"
#include "Enums.h"

ParameterKey::ParameterKey(const EBoardShape boardShape)
{
    key = TMap<FString, FString>
    {
        { INormal,  IVoid }, { IRift, IVoid }, { IWrap,  IVoid },
        { INoSpawn, IVoid }, { ILink, IVoid }, { ISwap,  IVoid },
        { IGap,	   IVoid }, { IIron, IVoid }, { ICloud, IVoid },
        { ICamo,    IVoid },
        { ICircuitWrap, IVoid },
        { IRotator,	   IString }, { ICorrectCounter,format({INumber, INumber}) },
        { IMirrorWrap,  IVoid },   { ITeleportWrap,  format({INumber, IString, INumber, INumber})},
    };
    
    switch (boardShape)
    {
    case EBoardShape::Square:
        key.Add(IBandaged, format({ICoord, ICoord}));
        key.Add(ICoord, format({INumber, INumber}));
        break;
    case EBoardShape::Hex:	    
    case EBoardShape::Triangle:
        key.Add(IBandaged, format({ICoord, ICoord, ICoord}));
        key.Add(ICoord, format({INumber, INumber, INumber}));
        break;
    default: throw std::invalid_argument("Invalid board shape.");
    }
}

bool ParameterKey::Contains(const FString& text)        { return key.Contains(text); }
FString& ParameterKey::operator[](const FString& index) { return key[index]; }
FString* ParameterKey::Find(const FString& parsedText)  { return key.Find(parsedText); }

FString ParameterKey::format(const std::initializer_list<FString> args)
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
