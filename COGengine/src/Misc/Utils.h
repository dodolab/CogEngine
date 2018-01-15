#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cmath>
#include "ofColor.h"
#include "Definitions.h"

using namespace std;

namespace Cog {


	// ======================== GLOBAL HELPER METHODS ==============================

	/**
	* Compares two floats with error tolerance
	*/
	inline bool isEqual(float a, float b)
	{
		return fabs(a - b) < 1E-12;
	}

	inline bool isEqualOrGreater(float a, float b) {
		return a > b || isEqual(a, b);
	}

	inline bool isEqualOrLower(float a, float b) {
		return a < b || isEqual(a, b);
	}

	/**
	* Creates formatted string
	*/
	string string_format(const char* fmt_str, ...);

	/**
	* Adds spaces into string stream
	*/
	void spaces(int howMany, ostream& ss);

	/**
	* Splits string
	*/
	void split_string(string str, string delimiter, vector<string>& output);

	/**
	* Returns true, if an event, executed at lastTime, with given frequency,
	* should be executed now
	*/
	bool CheckTime(uint64 lastTime, uint64 currentTime, float frequency);

	bool CheckTimeUnsigned(unsigned lastTime, unsigned currentTime, float frequency);

	int Modulo(int minValue, int maxValue, int currentValue);

	/**
	* Combines values into the hash
	*/
	inline int HashCombine(int hash, int value) {
		return (hash + (324723947 + value)) ^ 93485734985;
	}

} // namespace