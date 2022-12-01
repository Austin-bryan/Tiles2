#include "Logger.h"
#include <string>
#include "ParameterParseState.h"
#include "Coord.h"
#include "Vertex.h"

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
	const std::string s = c;
	logText += FString(c) + (s.find(':') ? "" : PAIR);
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
template <typename ... Types>
	void Log(const bool b, const Types&... types)
{
	logText += (b ? "true"_f : "false"_f) + PAIR;
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

FString operator+(const FString& lhs, const int rhs)     { return lhs + fstr(rhs); }
FString operator+(const FString& lhs, const float rhs)   { return lhs + fstr(rhs); }
FString operator+(const FString& lhs, const char* rhs)   { return lhs + fstr(rhs); }
FString operator+(const FString& lhs, const bool rhs)    { return lhs + fstr(rhs); }
FString operator+(const FString& lhs, const char rhs)    { return lhs + fstr(rhs); }
FString operator+(const FString& lhs, const FVector rhs) { return lhs + rhs.ToString(); }

inline std::ostringstream& operator<<(std::ostringstream& os, const FCoordPtr coord)
{
	os << coord->ToString();
	return os;
}
inline std::ostringstream& operator<<(std::ostringstream& os, const FParameter& coord)
{
	os << coord.ToString();
	return os;
}
inline std::ostringstream& operator<<(std::ostringstream& os, const Vertex& vertex)
{
	os << vertex.GetWorldPosition();
	return os;
}