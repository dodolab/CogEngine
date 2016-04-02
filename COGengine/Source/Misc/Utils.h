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


	// compares two floats with error tolerance
	inline bool isEqual(float a, float b)
	{
		if (fabs(a - b) < 1E-12)
		{
			return true;
		}

		return false;
	}

	/**
	* Creates formatted string
	*/
	string string_format(const char* fmt_str, ...);

	/**
	* Adds spaces into string stream
	*/
	void spaces(int howMany, ostream& ss);

	vector<string> split_string(string str, string delimiter);

	ofColor StringToColor(string col);

	bool IsProperTime(uint64 lastTime, uint64 actualTime, float frequency);

	inline int HashCombine(int hash, int value) {
		return (hash + (324723947 + value)) ^ 93485734985;
	}

} // namespace