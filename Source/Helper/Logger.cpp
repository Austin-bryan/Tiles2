#include "Logger.h"
#include <string>
#include "ParameterParseState.h"
#include "Coord.h"
#include "Vertex.h"

void AddOnScreenMessage(const FString string, const FColor color, const float time) { GEngine->AddOnScreenDebugMessage(-1, time, color, string); }

FString logText;
LogParams logParams;

template<typename... Types>
void UE_Log(const Types&... types) { UE_LOG(LogTemp, Warning, TEXT("%s"), *Format(types...)); }

template<typename... Types>
void Log(const Types&... types)
{
	auto string = Format(types...);
	AddOnScreenMessage(string, logParams.Color(), logParams.Time());
	logParams = LogParams();
}

FString Format()
{
	logText.RemoveFromEnd(PAIR);
	auto copy = logText;
	logText = "";
	
	return copy;
}
template <typename T, typename... Types>
	FString Format(const T& firstArg, const Types&... types)
{
	logText += fstr(firstArg) + PAIR;
	return Format(types...);
}
template<typename... Types>
	FString Format(const char* c, const Types&... types)
{
	const std::string s = c;
	logText += FString(c) + (s.find(':') ? "" : PAIR);
	return Format(types...);
}
template <typename... Types>
	FString Format(const LogParams& params, const Types&... types)
{
	logParams = params;
	return Format(types...);
}

template <typename  T>
FString fstr(const T& t)
{
    std::ostringstream oss;
	oss << t;
	return FString(oss.str().c_str());
}

template <> FString fstr<bool>(const bool& b) { return b ? "true"_f : "false"_f; }
FString fstr(const char* c) { return FString(c); }

void Path(const int n, const float time)					 			 { Path(n, defaultColor, time); }
void Path(const int n, const FColor color, const float time) 			 { AddOnScreenMessage(FString("Path: ") + FString::FromInt(n), color, time); }
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
FString operator+(const FString& lhs, const ATile* rhs)  { return lhs + rhs->ID(); }

FString operator+(const int     lhs, const FString& rhs) { return fstr(lhs) + rhs; }
FString operator+(const float   lhs, const FString& rhs) { return fstr(lhs) + rhs; }
FString operator+(const char*   lhs, const FString& rhs) { return fstr(lhs) + rhs; }
FString operator+(const bool    lhs, const FString& rhs) { return fstr(lhs) + rhs; }
FString operator+(const char    lhs, const FString& rhs) { return fstr(lhs) + rhs; }
FString operator+(const FVector lhs, const FString& rhs) { return lhs.ToString() + rhs; }
FString operator+(const ATile*  lhs, const FString& rhs) { return lhs->ID() + rhs; }

template <typename T>
std::ostringstream& CastEnum(std::ostringstream& os, T t, const FString& enumName)
{
	auto moduleName = UEnum::GetValueAsName(t).ToString();
	moduleName.RemoveFromStart("E"_f + enumName + "::"_f);
	os << moduleName;
	return os;
}

inline std::ostringstream& operator<<(std::ostringstream& os, const FString& string)   { os << static_cast<std::string>(TCHAR_TO_UTF8(*string)); return os; }
inline std::ostringstream& operator<<(std::ostringstream& os, const FVector& string)   { os << string.ToString();		  return os; }
inline std::ostringstream& operator<<(std::ostringstream& os, const FRotator& rotator) { os << rotator.ToString();		  return os; }
inline std::ostringstream& operator<<(std::ostringstream& os, const FColor& color)     { os << color.ToString();		  return os; }
inline std::ostringstream& operator<<(std::ostringstream& os, const FCoordPtr coord)   { os << coord->ToString();		  return os; }
inline std::ostringstream& operator<<(std::ostringstream& os, const FParameter& coord) { os << coord.ToString();		  return os; }
inline std::ostringstream& operator<<(std::ostringstream& os, const Vertex& vertex)    { os << vertex.GetWorldPosition(); return os; }
inline std::ostringstream& operator<<(std::ostringstream& os, const ATile* tile)       { os << tile->ID();				  return os; }
inline std::ostringstream& operator<<(std::ostringstream& os, const Vertex* v)         { os << (v == nullptr ? "null vertex"_f :  fstr(v->index)); return os; }

inline std::ostringstream& operator<<(std::ostringstream& os, const EModule e)         { return CastEnum(os, e, "Module"); }
inline std::ostringstream& operator<<(std::ostringstream& os, const ETileColor e)      { return CastEnum(os, e, "TileColor"); }
inline std::ostringstream& operator<<(std::ostringstream& os, const ESelectionAngle e) { return CastEnum(os, e, "SelectionAngle"); }
inline std::ostringstream& operator<<(std::ostringstream& os, const ESelectionShape e) { return CastEnum(os, e, "SelectionShape"); }
inline std::ostringstream& operator<<(std::ostringstream& os, const EBoardShape e)     { return CastEnum(os, e, "BoardShape"); }
inline std::ostringstream& operator<<(std::ostringstream& os, const ETriSeed e)        { return CastEnum(os, e, ""); }
inline std::ostringstream& operator<<(std::ostringstream& os, const EHexSeed e)        { return CastEnum(os, e, ""); }
inline std::ostringstream& operator<<(std::ostringstream& os, const ESqrSeed e)        { return CastEnum(os, e, ""); }
