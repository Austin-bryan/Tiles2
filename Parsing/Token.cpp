#include "Parsing/Token.h"
#include <ctype.h>
#include "Logger.h"

bool Token::CreateToken(const char c, Token& result)
{
	if (!(isalnum(c) || Tokens.Contains(fstr(c))))
		return false;
	result = Token(isdigit(c)
		? Token(TDigit, c)
		: isalpha(c)
		? Token(TAlpha, c)
		: Token(c));
	return true;
}
Token::Token(const char type, const char value) : type{type}, value{value}
{

}
FString Token::ToString() const
{
	return value == '\0' ? FString(1, &type) : FString(1, &type) + fstr(": ") + FString(1, &value);
}