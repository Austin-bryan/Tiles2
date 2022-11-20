#include "Logger.h"
#include <string>
#include "ParameterParseState.h"
#include "Coord.h"

void AddOnScreenMessage(const FString string, const FColor color, const float time) { GEngine->AddOnScreenDebugMessage(-1, time, color, string); }

FString logText;
LogParams logParams;

void Log()
{
	logText.RemoveFromEnd(PAIR);
	AddOnScreenMessage(logText, logParams.Color(), logParams.Time());
	logText = "";
	logParams = LogParams();
}

template<typename... Types>
void Log(const char* c, const Types&... types)
{
	logText += FString(c) + PAIR;
	Log(types...);
}

template <typename T, typename... Types>
void Log(const T& firstArg, const Types&... types)
{
	std::ostringstream oss;
	oss << firstArg;
	
	logText += FString(oss.str().c_str()) + PAIR;
	Log(types...);
}
template <typename... Types>
void Log(const LogParams& params, const Types&... types)
{
	logParams = params;
	Log(types...);
}

void Path(const int n, const float time) { Path(n, defaultColor, time); }
void Path(const int n, const FColor color, const float time) { AddOnScreenMessage(FString("Path: ") + FString::FromInt(n), color, time); }

void NullCheck(const void* object, const FColor color, const float time) { NullCheck("object", object, color, time); }
void NullCheck(FString&& label, const void* object, const FColor color, const  float time)
{
	AddOnScreenMessage(object == nullptr ? label + fstr(" is null") : label + fstr(" is not null"),
		color == defaultColor
		? object == nullptr
			? FColor::Red
			: FColor::Green
		: color, time);
}

inline FString operator+(const FString& lhs, const int rhs)     { return lhs + fstr(rhs); }
inline FString operator+(const FString& lhs, const float rhs)   { return lhs + fstr(rhs); }
inline FString operator+(const FString& lhs, const char* rhs)   { return lhs + fstr(rhs); }
inline FString operator+(const FString& lhs, const bool rhs)    { return lhs + fstr(rhs); }
inline FString operator+(const FString& lhs, const char rhs)    { return lhs + fstr(rhs); }
inline FString operator+(const FString& lhs, const FVector rhs) { return lhs + rhs.ToString(); }

std::ostringstream& operator<<(std::ostringstream& os, const FCoordPtr coord)
{
	os << coord->ToString();
	return os;
}
inline std::ostringstream& operator<<(std::ostringstream& os, const FParameter& coord)
{
	os << coord.ToString();
	return os;
}

//96
