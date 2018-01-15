
#include "EasingFunc.h"
#include "ofConstants.h"

namespace Cog {

	FadeFunction EasingFunc::linear = [](float x) {
		return x;
	};

	FadeFunction EasingFunc::elasticIn = [](float x) {
		return 1.0f - elasticOut(1.0f - x);
	};

	FadeFunction EasingFunc::elasticOut = [](float x) {
		float s = 1 - x;
		return 1 - powf(s, 8) + sinf(x * x * 6 * PI) * s * s;
	};

	FadeFunction EasingFunc::elasticInOut = [](float x) {
		return (x < 0.5f) ? elasticIn(x * 2.0f) * 0.5f : 1 - elasticIn(2.0f - x * 2.0f) * 0.5f;
	};

	FadeFunction EasingFunc::expoIn = [](float x) {
		return powf(2, 10 * (x - 1));
	};

	FadeFunction EasingFunc::expoOut = [](float x) {
		return 1.0f - powf(2, -10 * x);
	};

	FadeFunction EasingFunc::expoInOut = [](float x) {
		return (x < 0.5f) ? expoIn(x * 2.0f) * 0.5f : 1 - expoIn(2.0f - x * 2.0f) * 0.5f;
	};

	FadeFunction EasingFunc::quadIn = [](float x) {
		return x * x;
	};

	FadeFunction EasingFunc::quadOut = [](float x) {
		return 1 - (1-x)*(1-x);
	};

	FadeFunction EasingFunc::quadInOut = [](float x) {
		return (x < 0.5f) ? quadIn(x * 2.0f) * 0.5f : 1 - quadIn(2.0f - x * 2.0f) * 0.5f;
	};

	FadeFunction EasingFunc::cosineIn = [](float x) {
		return 1.0f - cosf(x * HALF_PI);
	};

	FadeFunction EasingFunc::cosineOut = [](float x) {
		return cosf((1-x)*HALF_PI);
	};

	FadeFunction EasingFunc::cosineInOut = [](float x) {
		return (x < 0.5f) ? cosineIn(x * 2.0f) * 0.5f : 1 - cosineIn(2.0f - x * 2.0f) * 0.5f;
	};

	FadeFunction EasingFunc::sinIn = [](float x) {
		return sinf(x * HALF_PI);
	};

	FadeFunction EasingFunc::sinOut = [](float x) {
		return sinf((1 - x)*HALF_PI);
	};

	FadeFunction EasingFunc::sinInOut = [](float x) {
		return (x < 0.5f) ? sinIn(x * 2.0f) * 0.5f : 1 - sinIn(2.0f - x * 2.0f) * 0.5f;
	};

} // namespace
