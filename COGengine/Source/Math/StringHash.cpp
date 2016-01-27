
#include "StringHash.h"


namespace Cog
{

	const StringHash StringHash::ZERO;

	StringHash::StringHash(const char* str) :
		value_(Calculate(str))
	{
#ifdef DEBUG
		StringHash::strValues[value_] = string(str);
#endif
	}

	StringHash::StringHash(const string& str) :
		value_(Calculate(str.c_str()))
	{
#ifdef DEBUG
		StringHash::strValues[value_] = str;
#endif
	}

	unsigned StringHash::Calculate(const char* str)
	{
		unsigned hash = 0;

		if (!str)
			return hash;

		while (*str)
		{
			// Perform the actual hashing as case-insensitive
			char c = *str;
			hash = SDBMHash(hash, (unsigned char)tolower(c));
			++str;
		}

		return hash;
	}

	string StringHash::ToString() const
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf_s(tempBuffer, "%08X", value_);
		return string(tempBuffer);
	}

#ifdef DEBUG
	map<unsigned,string> StringHash::strValues = map<unsigned, string>();
#endif
}