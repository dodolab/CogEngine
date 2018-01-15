#pragma once

#include "Definitions.h"
#include "Utils.h"
#include "Error.h"

using namespace std;


namespace Cog {


	/**
	* Container for easing functions
	* Each function takes a number in the range 0 to 1 and
	* returns a number in the same range 
	*/
	class EasingFunc {

	public:
		/** linear easing */
		static FadeFunction linear;
		/** smooth elastic in easing */
		static FadeFunction elasticIn;
		/** smooth elastic out easing */
		static FadeFunction elasticOut;
		/** smooth elastic in-out easing */
		static FadeFunction elasticInOut;
		/** exponential in easing */
		static FadeFunction expoIn;
		/** exponential out easing */
		static FadeFunction expoOut;
		/** exponential in-out easing */
		static FadeFunction expoInOut;
		/** quadratic in easing */
		static FadeFunction quadIn;
		/** quadratic out easing */
		static FadeFunction quadOut;
		/** quadratic in-out easing */
		static FadeFunction quadInOut;
		/** cosine in easing */
		static FadeFunction cosineIn;
		/** cosine out easing */
		static FadeFunction cosineOut;
		/** cosine in-out easing */
		static FadeFunction cosineInOut;
		/** sine in easing */
		static FadeFunction sinIn;
		/** sine out easing */
		static FadeFunction sinOut;
		/** sine in-out easing */
		static FadeFunction sinInOut;
		
	};

} // namespace
