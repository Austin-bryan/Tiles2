// ReSharper disable CppNonExplicitConvertingConstructor
// ReSharper disable CppNonExplicitConversionOperator
#pragma once
#include <optional>
#include "CoreMinimal.h"
#include "ParseState.h"
#include "Coord.h"
#include "ParameterRequesterParseState.h"

struct FCoord;
struct FSquareCoord;
struct FHexCoord;
struct FTriCoord;

/**
 * 
 */
struct FParameter
{
	FParameter(const float f);
	FParameter(const char* string);
	FParameter(const FString string);
	FParameter(FCoord* coord);

	template<typename T>
	const T& Get() const;

	template<typename T>
	const T* GetIf() const;

	FString ToString() const;
private:
	TVariant<float, FString, FCoord*> variant;
};

class TILES2_API ParameterParseState final : public ParameterRequesterParseState
{
public:
	explicit ParameterParseState(Parser& parser, const TSharedPtr<ParseState> parent);

	void OnPushed() override;
	void OnPopped() override;
	void ParseDelimiter() override;
	void ParseRightParen() override;
	void ParseLeftParen() override;
	// void AddParameter(FParameter&& parameter);
	void ParseExpectedTypes(const FString& parameterSeed, const bool waitForLeftParen = false);
	
	FString Name() const override;
	FString GetExpectedMessage() override;
private:
	void PushNextState();
	template<typename T>
	void ConstructParameter();
	void AddCoordParameter(EParameter childCoord, const FString& coordKey) const;

	std::optional<EParameter> parameterType;
	TArray<EParameter> expectedParameters;
	int parameterIndex = 0;
};
template <>
inline void ParameterParseState::ConstructParameter<FCoord*>();
