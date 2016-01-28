#pragma once

namespace Cog {

	/**
	* Creates formatted string
	*/
	std::string string_format(const char* fmt_str, ...);

	/**
	* Adds spaces into string stream
	*/
	void spaces(int howMany, std::ostream& ss);


} // namespace