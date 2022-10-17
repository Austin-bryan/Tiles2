#include "Logger.h"
#include <string>
#include "Parsing/ParseStates/ParameterParseState.h"

void Log(FString string, FColor color, float time)
{
	GEngine->AddOnScreenDebugMessage(-1, time, color, string);
}
void Log(int n,      FColor color, float time) { Log(FString::FromInt(n), color, time); }
void Log(float f,    FColor color, float time) { Log(FString::SanitizeFloat(f), color, time); }
void LogBool(bool b, FColor color, float time) { Log(fstr(b), color, time); }
void Log(FVector v,  FColor color, float time) { Log(v.ToString(), color, time); }
void Log(FRotator r, FColor color, float time) { Log(r.ToString(), color, time); }

void Log(FParameter& parameter, FColor color, float time) { Log(parameter.ToString(), color, time); }

void NullCheck(const void* object, FColor color, float time)
{
	NullCheck("object", object, color, time);
}
void NullCheck(FString&& label, const void* object, FColor color, float time)
{
	Log(object == nullptr ? label + fstr(" is null") : label + fstr(" is not null"), 
		object == nullptr ? FColor::Red : FColor::Green, time);
}

void Path(int n, float time, FColor color)
{
	Log(FString("Path: ") + FString::FromInt(n), color, time);
}
FString fstr(const char* c) { return FString(c); }
FString fstr(float f)       { return FString::SanitizeFloat(f); }
FString fstr(int i)         { return FString::FromInt(i); }
FString fstr(bool b)        { return b ? FString("true") : FString("false"); }
FString fstr(char c)		
{
	//return FString(tocstr(c));
	std::string s(1, c);
	return FString(s.c_str()); 
}