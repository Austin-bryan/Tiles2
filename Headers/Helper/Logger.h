#pragma once
#include "CoreMinimal.h"
#include <sstream>
#include "ForwardDeclares.h"

#define NL FString("\n")
#define SPC FString(" ")
#define LIST FString(":        ")
#define PAIR FString(",        ")

#define RED LogParams(FColor::Red)
#define ORANGE LogParams(FColor::Orange)
#define YELLOW LogParams(FColor::Yellow)
#define GREEN LogParams(FColor::Green)
#define CYAN LogParams(FColor::Cyan)
#define BLUE LogParams(FColor::Blue)
#define WHITE LogParams(FColor::White)
#define BLACK LogParams(FColor::Black)
#define TURQUOISE LogParams(FColor::Turquoise)
#define EMERALD LogParams(FColor::Emerald)
#define MAGENTA LogParams(FColor::Magenta)
#define PURPLE LogParams(FColor::Purple)

enum class ETileColor : uint8;
enum class EModule : uint8;
class Vertex;
class LogParams;
struct FParameter;

// TODO:: have a template<...>Format() function that has the same interface as template<...>Log(),
//      : then internally, Log uses Format() to reuse code. This allows string formatting of the same functions and interface

constexpr float defaultTime = 600.0f;
const FColor defaultColor = FColor::Cyan;

class LogParams
{
public:
    LogParams()                                                            : color{ defaultColor }, time{ defaultTime } { }
    explicit LogParams(const float time)                                   : color{ defaultColor }, time{ time } { }
    explicit LogParams(const FColor color, const float time = defaultTime) : color{ color }, time{ time } { }

    FColor Color() const { return color; }
    float Time()   const { return time; }
private:
    FColor color;
    float time;
};

void Log();
template<typename... Types>
    void Log(const char* c, const Types&... types);
template <typename T, typename... Types>
    void Log(const T& firstArg, const Types&... types);
template <typename... Types>
    void Log(const LogParams& params, const Types&... types);
template<typename... Types>
    void Log(const bool b, const Types&... types);

void Path(int n, float time);
void Path(int n, FColor color = defaultColor, float time = defaultTime);

void NullCheck(FString&& label, const void* object, FColor color = defaultColor, float time = defaultTime);
void NullCheck(const void* object, FColor color = defaultColor, float time = defaultTime);

inline FString operator"" _f(const long double f)            { return FString::SanitizeFloat(f); }
inline FString operator"" _f(const char* s, std::size_t)     { return FString(s); }
inline FString operator"" _f(const unsigned long long int i) { return FString::FromInt(i); }
inline FString operator"" _f(const char c)                   { return FString(std::string(1, c).c_str()); }

inline FString fstr(const float f) { return FString::SanitizeFloat(f); } 
inline FString fstr(const int i)   { return FString::FromInt(i); } 
inline FString fstr(const char* c) { return FString(c); } 
inline FString fstr(const bool b)  { return b ? FString("true") : FString("false"); } 
inline FString fstr(const char c)  { return FString(std::string(1, c).c_str()); }

FString operator+(const FString& lhs, const int rhs);
FString operator+(const FString& lhs, const float rhs);
FString operator+(const FString& lhs, const char* rhs);
FString operator+(const FString& lhs, const bool rhs);
FString operator+(const FString& lhs, const char rhs);
FString operator+(const FString& lhs, const FVector rhs);

FString operator+(const int     lhs, const FString& rhs);
FString operator+(const float   lhs, const FString& rhs);
FString operator+(const char*   lhs, const FString& rhs);
FString operator+(const bool    lhs, const FString& rhs);
FString operator+(const char    lhs, const FString& rhs);
FString operator+(const FVector lhs, const FString& rhs);

inline std::ostringstream& operator<<(std::ostringstream& os, const FString& string)
{
    os << static_cast<std::string>(TCHAR_TO_UTF8(*string));
    return os;
}
inline std::ostringstream& operator<<(std::ostringstream& os, const FVector& string)
{
    os << string.ToString();
    return os;
}
inline std::ostringstream& operator<<(std::ostringstream& os, const FRotator& rotator)
{
    os << rotator.ToString();
    return os;
}
inline std::ostringstream& operator<<(std::ostringstream& os, const FColor& color)
{
    os << color.ToString();
    return os;
}
inline std::ostringstream& operator<<(std::ostringstream& os, const FCoordPtr coord);
inline std::ostringstream& operator<<(std::ostringstream& os, const FParameter& coord);
inline std::ostringstream& operator<<(std::ostringstream& os, const Vertex& vertex);
inline std::ostringstream& operator<<(std::ostringstream& os, const EModule module);
inline std::ostringstream& operator<<(std::ostringstream& os, const ETileColor color);