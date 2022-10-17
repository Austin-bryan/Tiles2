#pragma once
#include "CoreMinimal.h"

#define NL fstr("\n")
#define SPC fstr(" ")
#define LIST fstr(": ")
#define PAIR fstr(", ")

struct FParameter;

const float defaultTime = 600.0f;
const FColor defaultColor = FColor::Cyan;

void Log(int,        FColor color = defaultColor, float time = defaultTime);
void Log(float,      FColor color = defaultColor, float time = defaultTime);
void LogBool(bool,   FColor color = defaultColor, float time = defaultTime);
void Log(FVector,    FColor color = defaultColor, float time = defaultTime);
void Log(FString,    FColor color = defaultColor, float time = defaultTime);	
void Log(FRotator,   FColor color = defaultColor, float time = defaultTime);
void Log(FParameter&, FColor color = defaultColor, float time = defaultTime);
void NullCheck(FString&& label, const void* object, FColor color = defaultColor, float time = defaultTime);
void NullCheck(const void* object, FColor color = defaultColor, float time = defaultTime);

void Path(int n, float time = defaultTime, FColor color = FColor::Cyan);

FString fstr(float f);
FString fstr(int i);
FString fstr(const char* c);
FString fstr(bool b);
FString fstr(char c);