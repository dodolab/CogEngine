#pragma once

#include "Definitions.h"
#include "Utils.h"
#include "Error.h"

using namespace std;


namespace Cog {


	// easing functions container
	class EasingManager {

	public:
		static FadeFunction linear;
		static FadeFunction elasticIn;
		static FadeFunction elasticOut;
		static FadeFunction elasticInOut;
		static FadeFunction expoIn;
		static FadeFunction expoOut;
		static FadeFunction expoInOut;
		static FadeFunction quadIn;
		static FadeFunction quadOut;
		static FadeFunction quadInOut;
		static FadeFunction cosineIn;
		static FadeFunction cosineOut;
		static FadeFunction cosineInOut;
		static FadeFunction sinIn;
		static FadeFunction sinOut;
		static FadeFunction sinInOut;
		
		static FadeFunction GetFadeFunction(string name) {
			if (name.compare("linear") == 0) {
				return linear;
			}
			if (name.compare("elastic_in") == 0) {
				return elasticIn;
			}
			if (name.compare("elastic_out") == 0) {
				return elasticOut;
			}
			if (name.compare("elastic") == 0) {
				return elasticInOut;
			}
			if (name.compare("expo_in") == 0) {
				return expoIn;
			}
			if (name.compare("expo_out") == 0) {
				return expoOut;
			}
			if (name.compare("expo") == 0) {
				return expoInOut;
			}
			if (name.compare("quad_in") == 0) {
				return quadIn;
			}
			if (name.compare("quad_out") == 0) {
				return quadOut;
			}
			if (name.compare("quad") == 0) {
				return quadInOut;
			}
			if (name.compare("cosine_in") == 0) {
				return cosineIn;
			}
			if (name.compare("cosine_out") == 0) {
				return cosineOut;
			}
			if (name.compare("cosine") == 0) {
				return cosineInOut;
			}
			if (name.compare("sin_in") == 0) {
				return sinIn;
			}
			if (name.compare("sin_out") == 0) {
				return sinOut;
			}
			if (name.compare("sin") == 0) {
				return sinInOut;
			}


			throw IllegalArgumentException(string_format("No such easing function : %s",name.c_str()));
		}
	};

} // namespace
