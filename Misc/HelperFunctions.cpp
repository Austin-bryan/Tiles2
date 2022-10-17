#include "Misc/HelperFunctions.h"
#include "Logger.h"

const char* tocstr(char c)
{
	std::string s(1, c);
	Log(fstr(c) + PAIR + fstr(s.c_str()), FColor::Red);
	return s.c_str();
}