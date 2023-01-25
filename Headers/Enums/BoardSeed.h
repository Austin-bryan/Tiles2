#pragma once
#include "CoreMinimal.h"
#include "BoardSeed.generated.h"

UENUM() enum class ESqrSeed : uint8 { Equal2, Equal3, Equal4, Equal5, Equal6, Equal7, Equal8, Equal9 };
UENUM() enum class ETriSeed : uint8 { Equal2, Equal3, Equal4, Equal5, Equal6, Equal7, Equal8, Equal9 };
UENUM() enum class EHexSeed : uint8 { Equal3, Equal5, Equal7, Equal9 };

inline FString ToString(ESqrSeed seed)
{
    switch (seed)
    {
    case ESqrSeed::Equal2: return "sqr (2, 2) | r/r/ b/b;";
    case ESqrSeed::Equal3: return "sqr (3, 3) | b/b/b/ g/g/g/ r/r/r;";
    case ESqrSeed::Equal4: return "sqr (4, 4) | r/r/r/r/ g/g/g/g/ b/b/b/b/ c/c/c/c;";
    case ESqrSeed::Equal5: return "sqr (5, 5) | r/r/r/r/r/ o/o/o/o/o/ g/g/g/g/g/ b/b/b/b/b/ c/c/c/c/c;";
    case ESqrSeed::Equal6: return "sqr (6, 6) | r/r/r/r/r/r/ o/o/o/o/o/o/ g/g/g/g/g/g/ b/b/b/b/b/b/ c/c/c/c/c/c/ y/y/y/y/y/y;";
    case ESqrSeed::Equal7: return "sqr (7, 7) | r/r/r/r/r/r/r/ o/o/o/o/o/o/o/ g/g/g/g/g/g/g/ b/b/b/b/b/b/b/ c/c/c/c/c/c/c/ y/y/y/y/y/y/y/ n/n/n/n/n/n/n;";
    case ESqrSeed::Equal8: return "sqr (8, 8) | r/r/r/r/r/r/r/r/ o/o/o/o/o/o/o/o/ y/y/y/y/y/y/y/y/ g/g/g/g/g/g/g/g/ c/c/c/c/c/c/c/c /b/b/b/b/b/b/b/b /p/p/p/p/p/p/p/p /n/n/n/n/n/n/n/n;";
    case ESqrSeed::Equal9: return "sqr (9, 9) | r/r/r/r/r/r/r/r/r/ o/o/o/o/o/o/o/o/o/ y/y/y/y/y/y/y/y/y/ g/g/g/g/g/g/g/g/g/ c/c/c/c/c/c/c/c/c /b/b/b/b/b/b/b/b/b /p/p/p/p/p/p/p/p/p /n/n/n/n/n/n/n/n/n /k/k/k/k/k/k/k/k/k;";
    default: return ""; ;
    }
}
inline FString ToString(ETriSeed seed)
{
    switch (seed)
    {
    case ETriSeed::Equal2: return "tri (2, 2, 2) | r/ b/b/b;";
    case ETriSeed::Equal3: return "tri (3, 3, 3) | r/ b/b/b/ g/g/g/g/g;";
    case ETriSeed::Equal4: return "tri (4, 4, 4) | r/ b/b/b/ g/g/g/g/g/ n/n/n/n/n/n/n;";
    case ETriSeed::Equal5: return "tri (5, 5, 5) | r/ b/b/b/ g/g/g/g/g/ n/n/n/n/n/n/n/ y/y/y/y/y/y/y/y/y;";
    case ETriSeed::Equal6: return "tri (6, 6, 6) | r/ b/b/b/ g/g/g/g/g/ n/n/n/n/n/n/n/ y/y/y/y/y/y/y/y/y/ c/c/c/c/c/c/c/c/c/c/c;";
    case ETriSeed::Equal7: return "tri (7, 7, 7) | r/ b/b/b/ g/g/g/g/g/ n/n/n/n/n/n/n/ y/y/y/y/y/y/y/y/y/ c/c/c/c/c/c/c/c/c/c/c/ o/o/o/o/o/o/o/o/o/o/o/o/o;";
    case ETriSeed::Equal8: return "tri (8, 8, 8) | r/ b/b/b/ g/g/g/g/g/ n/n/n/n/n/n/n/ y/y/y/y/y/y/y/y/y/ c/c/c/c/c/c/c/c/c/c/c/ o/o/o/o/o/o/o/o/o/o/o/o/o/ m/m/m/m/m/m/m/m/m/m/m/m/m/m/m;";
    case ETriSeed::Equal9: return "tri (9, 9, 9) | r/ b/b/b/ g/g/g/g/g/ n/n/n/n/n/n/n/ y/y/y/y/y/y/y/y/y/ c/c/c/c/c/c/c/c/c/c/c/ o/o/o/o/o/o/o/o/o/o/o/o/o/ m/m/m/m/m/m/m/m/m/m/m/m/m/m/m /k/k/k/k/k/k/k/k/k/k/k/k/k/k/k/k/k;";
    default: return "";
    }
}
inline FString ToString(EHexSeed seed)
{
    switch (seed)
    {
    case EHexSeed::Equal3: return "hex (3, 3, 3) | b/b/ g/g/g/ n/n;";
    case EHexSeed::Equal5: return "hex (5, 5, 5) | r/r/r/ b/b/b/b/ g/g/g/g/g/ n/n/n/n/ k/k/k;";
    case EHexSeed::Equal7: return "hex (7, 7, 7) | r/r/r/r/ b/b/b/b/b/ g/g/g/g/g/g/ n/n/n/n/n/n/n/ k/k/k/k/k/k/ p/p/p/p/p/ c/c/c/c;";
    case EHexSeed::Equal9: return "hex (9, 9, 9) | r/r/r/r/r/ b/b/b/b/b/b/ g/g/g/g/g/g/g/ n/n/n/n/n/n/n/n/ k/k/k/k/k/k/k/k/k/ p/p/p/p/p/p/p/p/ c/c/c/c/c/c/c/ o/o/o/o/o/o/ y/y/y/y/y;";
    default: return "";
    }
}