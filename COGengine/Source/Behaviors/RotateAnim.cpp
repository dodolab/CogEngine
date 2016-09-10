
#include "RotateAnim.h"
#include "CogEngine.h"

namespace Cog {


	void RotateAnim::Update(const uint64 delta, const uint64 absolute) {

		// calculate differencial
		float diff = (float)((to == from ? 1 : (to - from)) * 0.001f * speed * delta);
		actual += diff;

		// if to == from, it is infinite rotation
		if (to != from && (actual - from > to - from)) {
			actual = to;
			Finish();
		}

		Trans& transform = owner->GetTransform();

		if (additive) transform.rotation += (diff);
		else transform.rotation = (actual);
	}

}// namespace