#pragma once

#include <string>
#include <map>
#include "ofUtils.h"

using std::map;
using std::string;

namespace Cog
{


	/**
	* Entity that transforms string into unsigned value; is used for quicker access to hashmaps where
	* numbers are used as keys
	*/
	class StrId
	{
	public:

		StrId() : val(0){}

		StrId(const StrId& rhs) : val(rhs.val){}

		explicit StrId(unsigned value) : val(value){}

		// Construct from a C string
		StrId(const char* str);
		// Construct from a string
		StrId(const string& str);

		// Assign from another hash.
		StrId& operator =(const StrId& rhs)
		{
			val = rhs.val;
			return *this;
		}

		// Add a hash.
		StrId operator +(const StrId& rhs) const
		{
			StrId ret;
			ret.val = val + rhs.val;
			return ret;
		}

		// Add-assign a hash.
		StrId& operator +=(const StrId& rhs)
		{
			val += rhs.val;
			return *this;
		}

		bool operator ==(const StrId& rhs) const { return val == rhs.val; }
		bool operator !=(const StrId& rhs) const { return val != rhs.val; }
		bool operator <(const StrId& rhs) const { return val < rhs.val; }
		bool operator >(const StrId& rhs) const { return val > rhs.val; }

		// return unsigned value
		operator unsigned() const { return val; }

		// Return hash value.
		unsigned GetValue() const { return val; }

		// calculates hash from c string
		static unsigned Calculate(const char* str);

		static string GetStringValue(unsigned val) {
#ifdef _DEBUG
			auto fnd = strValues.find(val);

			if (fnd != strValues.end()) {
				return fnd->second;
			}
			return string("");
#else
			return ofToString(val);
#endif
		}

	private:
		// Hash value.
		unsigned val;

#ifdef _DEBUG
		// value-string map for logging, only for debug mode
		static map<unsigned, string>  strValues;
#endif

	};

} // namespace
