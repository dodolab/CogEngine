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


	void split_string(string str, string delimiter, std::vector<std::string>& output) {
		size_t pos = 0;
		std::string token;
		
		while ((pos = str.find(delimiter)) != std::string::npos) {
			token = str.substr(0, pos);
			output.push_back(token);
			str.erase(0, pos + delimiter.length());
		}
		output.push_back(str);
	}


	bool IsProperTime(uint64 lastTime, uint64 actualTime, float frequency) {
		if (lastTime > actualTime) return false;

		float period = 1000 / frequency;
		return (actualTime - lastTime) >= period;
	}


}// namespace