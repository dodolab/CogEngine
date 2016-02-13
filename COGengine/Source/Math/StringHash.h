#pragma once

#include <string>
#include <map>
#include "ofUtils.h"

using std::map;
using std::string;

namespace Cog
{

#define CONVERSION_BUFFER_LENGTH 128


	/**
	* Entity that transforms string into unsigned value; is used for quicker access to hashmaps where
	* numbers are used as keys
	*/
	class StringHash
	{
	public:
		// Construct with zero value.
		StringHash() :
			value_(0)
		{
		}

		// Copy-construct from another hash.
		StringHash(const StringHash& rhs) :
			value_(rhs.value_)
		{
		}

		// Construct with an initial value.
		explicit StringHash(unsigned value) :
			value_(value)
		{
		}

		// Construct from a C string case-insensitively.
		StringHash(const char* str);
		// Construct from a string case-insensitively.
		StringHash(const string& str);

		// Assign from another hash.
		StringHash& operator =(const StringHash& rhs)
		{
			value_ = rhs.value_;
			return *this;
		}

		// Add a hash.
		StringHash operator +(const StringHash& rhs) const
		{
			StringHash ret;
			ret.value_ = value_ + rhs.value_;
			return ret;
		}

		// Add-assign a hash.
		StringHash& operator +=(const StringHash& rhs)
		{
			value_ += rhs.value_;
			return *this;
		}

		// Test for equality with another hash.
		bool operator ==(const StringHash& rhs) const { return value_ == rhs.value_; }

		// Test for inequality with another hash.
		bool operator !=(const StringHash& rhs) const { return value_ != rhs.value_; }

		// Test if less than another hash.
		bool operator <(const StringHash& rhs) const { return value_ < rhs.value_; }

		// Test if greater than another hash.
		bool operator >(const StringHash& rhs) const { return value_ > rhs.value_; }

		// return unsigned value
		operator unsigned() const { return value_; }

		// Return hash value.
		unsigned Value() const { return value_; }

		// Return as string.
		string ToString() const;

		// Return hash value for HashSet & HashMap.
		unsigned ToHash() const { return value_; }

		// Calculate hash value case-insensitively from a C string.
		static unsigned Calculate(const char* str);

		// Zero hash.
		static const StringHash ZERO;


		static string GetStringValue(unsigned val) {
#ifdef DEBUG
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
		unsigned value_;

#ifdef DEBUG
		// value-string map for logging, only for debug mode
		static map<unsigned, string>  strValues;
#endif

	};

	// Update a hash with the given 8-bit value using the SDBM algorithm.
	inline unsigned SDBMHash(unsigned hash, unsigned char c) { return c + (hash << 6) + (hash << 16) - hash; }

} // namespace
