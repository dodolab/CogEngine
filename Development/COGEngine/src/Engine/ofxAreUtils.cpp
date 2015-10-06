#include "ofUtils.h"


string string_format(const char* fmt_str, ...){
	va_list args;
	va_start(args, fmt_str);
	string output = ofVAArgsToString(fmt_str, args);
	va_end(args);
	return output;
}

void spaces(int howMany, ostream& ss){
	for (int i = 0; i < howMany; i++){
		ss << " ";
	}
}