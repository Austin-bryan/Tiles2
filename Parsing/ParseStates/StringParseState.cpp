#include "StringParseState.h"

#include "Logger.h"
#include "ParameterParseState.h"
#include "Parser.h"

StringParseState::StringParseState(Parser& parser, TSharedPtr<ParseState> parent) : ParseState(parser, parent){}
StringParseState::~StringParseState(){}

void StringParseState::ParseAlpha(const char c)
{
    if (!inQuotes)
    {
        parser.Throw(fstr(c), "Expected a string.");
        return;
    }
    parsedText += c;
}
void StringParseState::ParseQuotation()
{
    if (inQuotes)
    {
        if (parsedText == "")
             parser.Throw(fstr(TQuotation), "Empty strings are not allowed.");
        else
        {
            StaticCastSharedPtr<ParameterParseState>(parent)->AddParameter(parsedText);
            PopState();
        }
    }
    else inQuotes = true;
}
void StringParseState::ParseRightParen() { parser.Throw(TRightParen, "Expected '\"'"); }

FString StringParseState::Name() const
{
    return "String State";
}
