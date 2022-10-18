#pragma once
#include "CoreMinimal.h"

#define NL fstr("\n")
#define SPC fstr(" ")
#define LIST fstr(": ")
#define PAIR fstr(", ")

struct FParameter;

constexpr float defaultTime = 600.0f;
const FColor defaultColor = FColor::Cyan;

void Log(const int,      const FColor color = defaultColor, const float time = defaultTime);
void Log(const float,    const FColor color = defaultColor, const float time = defaultTime);
void Log(const FVector,  const FColor color = defaultColor, const float time = defaultTime);
void Log(const FString,  const FColor color = defaultColor, const float time = defaultTime);	
void Log(const FRotator, const FColor color = defaultColor, const float time = defaultTime);
void Log(const FParameter&, const FColor color = defaultColor, const float time = defaultTime);
void LogBool(bool, const FColor color = defaultColor, const float time = defaultTime);
void NullCheck(FString&& label, const void* object, const FColor color = defaultColor, const float time = defaultTime);
void NullCheck(const void* object, FColor color = defaultColor, float time = defaultTime);

void Path(const int n, const float time = defaultTime, const FColor color = FColor::Cyan);

FString fstr(const float f);
FString fstr(const int i);
FString fstr(const char* c);
FString fstr(const bool b);
FString fstr(const char c);