#pragma once
#include "CoreMinimal.h"

#define NL fstr("\n")
#define SPC fstr(" ")
#define LIST fstr(": ")
#define PAIR fstr(", ")

struct FParameter;

constexpr float defaultTime = 600.0f;
const FColor defaultColor = FColor::Cyan;

void Log(int,         FColor color = defaultColor, float time = defaultTime);
void Log(float,       FColor color = defaultColor, float time = defaultTime);
void Log(FVector,     FColor color = defaultColor, float time = defaultTime);
void Log(FString,     FColor color = defaultColor, float time = defaultTime);	
void Log(FRotator,    FColor color = defaultColor, float time = defaultTime);
void Log(FParameter&, FColor color = defaultColor, float time = defaultTime);

void NullCheck(FString&& label, const void* object, FColor color = defaultColor, float time = defaultTime);
void NullCheck(const void* object, FColor color = defaultColor, float time = defaultTime);

void Path(const int n, const float time = defaultTime, const FColor color = FColor::Cyan);

inline FString operator"" _f(const unsigned long long int i) { return FString::FromInt(i); }
inline FString operator"" _f(const char* s, std::size_t) { return FString(s); }
inline FString operator"" _f(const long double f) { return FString::SanitizeFloat(f); }
inline FString operator"" _f(const char c)
{
    const std::string s(1, c);
    return FString(s.c_str()); 
}

FString fstr(const float f);
FString fstr(const int i);
FString fstr(const char* c);
FString fstr(const bool b);
FString fstr(const char c);

inline FString operator+(const FString& lhs, const int rhs);
inline FString operator+(const FString& lhs, const float rhs);
inline FString operator+(const FString& lhs, const char* rhs);
inline FString operator+(const FString& lhs, const bool rhs);
inline FString operator+(const FString& lhs, const char rhs);
inline FString operator+(const FString& lhs, const FVector rhs);