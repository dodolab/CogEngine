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


	bool CheckTime(uint64 lastTime, uint64 currentTime, float frequency) {
		if (lastTime > currentTime) return false;

		float period = 1000 / frequency;
		return (currentTime - lastTime) >= period;
	}

	bool CheckTimeUnsigned(unsigned lastTime, unsigned currentTime, float frequency) {
		if (lastTime > currentTime) return false;

		float period = 1000 / frequency;
		return (currentTime - lastTime) >= period;
	}

	int Modulo(int minValue, int maxValue, int currentValue) {
		return (((int)ofClamp(currentValue, minValue, maxValue)) - minValue + 1) % (maxValue - minValue + 1) + minValue;
	}
}// namespace