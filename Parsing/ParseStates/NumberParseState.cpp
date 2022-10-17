#include "NumberParseState.h"

#include "ModuleParseState.h"
#include "ParameterParseState.h"
#include "Parser.h"

NumberParseState::NumberParseState(Parser& parser, TSharedPtr<ParseState> parent) : ParseState(parser, parent) {}
NumberParseState::~NumberParseState(){}

void NumberParseState::ParseNumber(const int n)
{
    if (sign == 0)
        sign = 1;
    if (isFloat)
         parsedDecimals = parsedDecimals * 10 + n;
    else parsedIntegers = parsedIntegers * 10 + n;
}
void NumberParseState::ParseNegation()
{
    if (sign == 0)
        sign = -1;
    else parser.Throw(TNegation, "Invalid number format.");
}
void NumberParseState::ParsePeriod()
{
    if (!isFloat)
         isFloat = true;
    else parser.Throw(TPeriod, "Invalid number format.");
}
void NumberParseState::ParseDelimiter()
{
    ExitNumberState();
    StaticCastSharedPtr<ParameterParseState>(parent)->ParseDelimiter();
}
void NumberParseState::ParseRightParen()
{
    ExitNumberState();
    StaticCastSharedPtr<ParameterParseState>(parent)->ParseRightParen();
}
FString NumberParseState::Name() const
{
    return "Number State";
}

void NumberParseState::ExitNumberState() const
{
    if (sign == 0)
    {
        parser.Throw(TRightParen, "Expected a number.");
        return;
    }
    const int digitCount = fstr(parsedDecimals).Len();
    const float decimals = parsedDecimals / pow(10, digitCount);
    const float number   = sign * RoundToPlaces(parsedIntegers + decimals, digitCount);

    StaticCastSharedPtr<ParameterParseState>(parent)->AddParameter(number);
    PopState();
}
float NumberParseState::RoundToPlaces(const float f, const int decimalCount) const
{
    const float mult = std::pow(10.0f, decimalCount);
    return std::round(f * mult) / mult;
}

FString NumberParseState::GetExpectedMessage() { return "Expected a number."; }

