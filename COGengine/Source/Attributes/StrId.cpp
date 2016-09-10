
#include "StrId.h"

namespace Cog
{

	StrId::StrId(const char* str) :
		val(Calculate(str))
	{
#ifdef _DEBUG
		StrId::strValues[val] = string(str);
		this->strVal = str;
#endif
	}

	StrId::StrId(const string& str) :
		val(Calculate(str.c_str()))
	{
#ifdef _DEBUG
		StrId::strValues[val] = str;
		this->strVal = str;
#endif
	}

	unsigned StrId::Calculate(const char* str)
	{
		unsigned hash = 0;

		if (!str) return hash;

		while (*str)
		{
			// SDBM hash is used, see http://www.cse.yorku.ca/~oz/hash.html
			char c = tolower(*str); // always get lowercase
			hash = ((unsigned char)c) + (hash << 6) + (hash << 16) - hash;
			++str;
		}

		return hash;
	}


#ifdef _DEBUG
	map<unsigned,string> StrId::strValues = map<unsigned, string>();
#endif
}