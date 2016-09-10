#include "Utils.h"
#include <stdarg.h>  
#include "ofUtils.h"

namespace Cog {

	

	string string_format(const char* fmt_str, ...) {
		va_list args;
		va_start(args, fmt_str);
		string output = ofVAArgsToString(fmt_str, args);
		va_end(args);
		return output;
	}

	void spaces(int howMany, ostream& ss) {
		for (int i = 0; i < howMany; i++) {
			ss << " ";
		}
	}


	std::vector<std::string> split_string(string str, string delimiter) {
		size_t pos = 0;
		std::string token;
		vector<string> output = vector<string>();

		while ((pos = str.find(delimiter)) != std::string::npos) {
			token = str.substr(0, pos);
			output.push_back(token);
			str.erase(0, pos + delimiter.length());
		}
		output.push_back(str);
		return output;
	}

}// namespace