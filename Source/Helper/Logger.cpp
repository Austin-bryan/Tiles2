#include "Logger.h"
#include <string>
#include "ParseStates/ParameterParseState.h"

void Log(const int n,      const FColor color, const float time) { Log(FString::FromInt(n), color, time); }
void Log(const float f,    const FColor color, const float time) { Log(FString::SanitizeFloat(f), color, time); }
void LogBool(const bool b, const FColor color, const float time) { Log(fstr(b), color, time); }
void Log(const FVector v,  const FColor color, const float time) { Log(v.ToString(), color, time); }
void Log(const FRotator r, const FColor color, const float time) { Log(r.ToString(), color, time); }

void Log(const FParameter& parameter, const  FColor color, const  float time) { Log(parameter.ToString(), color, time); }

void NullCheck(const void* object, const FColor color, const float time)
{
	NullCheck("object", object, color, time);
}
void NullCheck(FString&& label, const void* object, FColor color, const  float time)
{
	Log(object == nullptr ? label + fstr(" is null") : label + fstr(" is not null"), 
		object == nullptr ? FColor::Red : FColor::Green, time);
}

void Path(const int n, const float time, const FColor color)
{
	Log(FString("Path: ") + FString::FromInt(n), color, time);
}
FString fstr(const char* c) { return FString(c); }
FString fstr(const float f) { return FString::SanitizeFloat(f); }
FString fstr(const int i)   { return FString::FromInt(i); }
FString fstr(const bool b)  { return b ? FString("true") : FString("false"); }
FString fstr(const char c)		
{
	const std::string s(1, c);
	return FString(s.c_str()); 
}

inline FString operator+(const FString& lhs, const int rhs)     { return lhs + fstr(rhs); }
inline FString operator+(const FString& lhs, const float rhs)   { return lhs + fstr(rhs); }
inline FString operator+(const FString& lhs, const char* rhs)   { return lhs + fstr(rhs); }
inline FString operator+(const FString& lhs, const bool rhs)    { return lhs + fstr(rhs); }
inline FString operator+(const FString& lhs, const char rhs)    { return lhs + fstr(rhs); }
inline FString operator+(const FString& lhs, const FVector rhs) { return lhs + rhs.ToString(); }
