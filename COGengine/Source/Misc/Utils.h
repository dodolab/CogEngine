#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

namespace Cog {

	/**
	* Creates formatted string
	*/
	string string_format(const char* fmt_str, ...);

	/**
	* Adds spaces into string stream
	*/
	void spaces(int howMany, ostream& ss);

	vector<string> split_string(string str, string delimiter);


} // namespace