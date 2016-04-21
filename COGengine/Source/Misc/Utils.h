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
	* Returns true, if it is proper time to execute an action according to the frequency 
	* the action should be executed
	* @param lastTime the last time the action has been executed
	* @param actualTime current time
	* @param frequency the frequency the action should be executed
	*/
	bool IsProperTime(uint64 lastTime, uint64 actualTime, float frequency);

	/**
	* Combines values into the hash
	*/
	inline int HashCombine(int hash, int value) {
		return (hash + (324723947 + value)) ^ 93485734985;
	}

} // namespace