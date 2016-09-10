
#include "EasingFunc.h"
#include "ofConstants.h"

namespace Cog {

	FadeFunction EasingManager::linear = [](float x) {
		return x;
	};

	FadeFunction EasingManager::elasticIn = [](float x) {
		return 1.0f - elasticOut(1.0f - x);
	};

	FadeFunction EasingManager::elasticOut = [](float x) {
		float s = 1 - x;
		return 1 - powf(s, 8) + sinf(x * x * 6 * PI) * s * s;
	};

	FadeFunction EasingManager::elasticInOut = [](float x) {
		return (x < 0.5f) ? elasticIn(x * 2.0f) * 0.5f : 1 - elasticIn(2.0f - x * 2.0f) * 0.5f;
	};

	FadeFunction EasingManager::expoIn = [](float x) {
		return powf(2, 10 * (x - 1));
	};

	FadeFunction EasingManager::expoOut = [](float x) {
		return 1.0f - powf(2, -10 * x);
	};

	FadeFunction EasingManager::expoInOut = [](float x) {
		return (x < 0.5f) ? expoIn(x * 2.0f) * 0.5f : 1 - expoIn(2.0f - x * 2.0f) * 0.5f;
	};

	FadeFunction EasingManager::quadIn = [](float x) {
		return x * x;
	};

	FadeFunction EasingManager::quadOut = [](float x) {
		return 1 - (1-x)*(1-x);
	};

	FadeFunction EasingManager::quadInOut = [](float x) {
		return (x < 0.5f) ? quadIn(x * 2.0f) * 0.5f : 1 - quadIn(2.0f - x * 2.0f) * 0.5f;
	};

	FadeFunction EasingManager::cosineIn = [](float x) {
		return 1.0f - cosf(x * HALF_PI);
	};

	FadeFunction EasingManager::cosineOut = [](float x) {
		return cosf((1-x)*HALF_PI);
	};

	FadeFunction EasingManager::cosineInOut = [](float x) {
		return (x < 0.5f) ? cosineIn(x * 2.0f) * 0.5f : 1 - cosineIn(2.0f - x * 2.0f) * 0.5f;
	};

	FadeFunction EasingManager::sinIn = [](float x) {
		return sinf(x * HALF_PI);
	};

	FadeFunction EasingManager::sinOut = [](float x) {
		return sinf((1 - x)*HALF_PI);
	};

	FadeFunction EasingManager::sinInOut = [](float x) {
		return (x < 0.5f) ? sinIn(x * 2.0f) * 0.5f : 1 - sinIn(2.0f - x * 2.0f) * 0.5f;
	};

} // namespace
