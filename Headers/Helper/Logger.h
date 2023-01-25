#pragma once
#include "CoreMinimal.h"
#include <sstream>
#include "ForwardDeclares.h"

#define PRINT(x) Format(FString(#x), + x) 

#define NL   FString("\n")
#define SPC  FString(" ")
#define LIST FString(":  ")
#define PAIR FString(",  ")

#define RED       LogParams(FColor::Red)
#define ORANGE    LogParams(FColor::Orange)
#define YELLOW    LogParams(FColor::Yellow)
#define GREEN     LogParams(FColor::Green)
#define CYAN      LogParams(FColor::Cyan)
#define BLUE      LogParams(FColor::Blue)
#define WHITE     LogParams(FColor::White)
#define BLACK     LogParams(FColor::Black)
#define TURQUOISE LogParams(FColor::Turquoise)
#define EMERALD   LogParams(FColor::Emerald)
#define MAGENTA   LogParams(FColor::Magenta)
#define PURPLE    LogParams(FColor::Purple)
#define TICK      LogParams(0)

enum class EBoardShape;
enum class EModule : uint8;
enum class ESqrSeed : uint8;
enum class EHexSeed : uint8;
enum class ETriSeed : uint8;
enum class ETileColor : uint8;
enum class ESelectionAngle : uint8;
enum class ESelectionShape : uint8;
class Vertex;
class LogParams;
struct FParameter;


// TODO:: Add Format<> Function
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

template<typename... Types>
void UE_Log(const Types&... types);

template <typename... Types>
    void Log(const Types&... types);

FString Format();
template <typename T, typename... Types>
    FString Format(const T& firstArg, const Types&... types);
template<typename... Types>
    FString Format(const char* c, const Types&... types);
template <typename... Types>
    FString Format(const LogParams& params, const Types&... types);

void Path(int n, float time);
void Path(int n, FColor color = defaultColor, float time = defaultTime);
void NullCheck(FString&& label, const void* object, FColor color = defaultColor, float time = defaultTime);
void NullCheck(const void* object, FColor color = defaultColor, float time = defaultTime);

inline FString operator"" _f(const long double f)            { return FString::SanitizeFloat(f); }
inline FString operator"" _f(const char* s, std::size_t)     { return FString(s); }
inline FString operator"" _f(const unsigned long long int i) { return FString::FromInt(i); }
inline FString operator"" _f(const char c)                   { return FString(std::string(1, c).c_str()); }

template <class T>
FString fstr(const T& t);

template <> FString fstr<bool>(const bool& b);
FString fstr(const char* c);

template <class T>
FText ftxt(T t) { return FText::FromString(fstr(t)); }

FString operator+(const FString& lhs, const int rhs);
FString operator+(const FString& lhs, const float rhs);
FString operator+(const FString& lhs, const char* rhs);
FString operator+(const FString& lhs, const bool rhs);
FString operator+(const FString& lhs, const char rhs);
FString operator+(const FString& lhs, const FVector rhs);
FString operator+(const FString& lhs, const ATile* rhs);

FString operator+(const int     lhs, const FString& rhs);
FString operator+(const float   lhs, const FString& rhs);
FString operator+(const char*   lhs, const FString& rhs);
FString operator+(const bool    lhs, const FString& rhs);
FString operator+(const char    lhs, const FString& rhs);
FString operator+(const FVector lhs, const FString& rhs);
FString operator+(const ATile*  lhs, const FString& rhs);

template<typename T>
std::ostringstream& CastEnum(std::ostringstream& os, T t, const FString& enumName);
inline std::ostringstream& operator<<(std::ostringstream& os, const FString& string);
inline std::ostringstream& operator<<(std::ostringstream& os, const FVector& string);
inline std::ostringstream& operator<<(std::ostringstream& os, const FRotator& rotator);
inline std::ostringstream& operator<<(std::ostringstream& os, const FColor& color);
inline std::ostringstream& operator<<(std::ostringstream& os, const FCoordPtr coord);
inline std::ostringstream& operator<<(std::ostringstream& os, const FParameter& coord);
inline std::ostringstream& operator<<(std::ostringstream& os, const Vertex& vertex);
inline std::ostringstream& operator<<(std::ostringstream& os, const ATile* tile);
inline std::ostringstream& operator<<(std::ostringstream& os, const Vertex* v);

inline std::ostringstream& operator<<(std::ostringstream& os, const EModule e);
inline std::ostringstream& operator<<(std::ostringstream& os, const ETileColor e);
inline std::ostringstream& operator<<(std::ostringstream& os, const ESelectionAngle e);
inline std::ostringstream& operator<<(std::ostringstream& os, const ESelectionShape e);
inline std::ostringstream& operator<<(std::ostringstream& os, const EBoardShape e);
inline std::ostringstream& operator<<(std::ostringstream& os, const ETriSeed e);
inline std::ostringstream& operator<<(std::ostringstream& os, const EHexSeed e);
inline std::ostringstream& operator<<(std::ostringstream& os, const ESqrSeed e);