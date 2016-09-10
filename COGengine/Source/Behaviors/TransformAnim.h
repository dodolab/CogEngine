#pragma once

#include "Behavior.h"
#include "Node.h"

namespace Cog {

	/**x
	* Behavior for common transformation animation
	*/
	class TransformAnim : public Behavior {
		OBJECT_PROTOTYPE(TransformAnim)
	private:
		// duration in ms
		float duration = 0;
		// starting transform
		Trans to = Trans();
		// ending transform
		Trans from = Trans();
		// actual duration
		float actual = 0;
		// fade function
		FadeFunction fadeFunction;
		float delayAfter = 0;

	public:

		TransformAnim(Trans from, Trans to, float duration, float delayAfter) : to(to), from(from), duration(duration),
			delayAfter(delayAfter) {
			fadeFunction = nullptr;
		}

		TransformAnim(Trans from, Trans to, float duration, float delayAfter, FadeFunction fadeFunction) :
			to(to), from(from), duration(duration), delayAfter(delayAfter) {
			this->fadeFunction = fadeFunction;
		}

		void Update(const uint64 delta, const uint64 absolute) {

			actual += delta;

			float actualCropped = actual;

			if (actual >= duration) {
				actualCropped = duration;
			}

			float actualPercent = actualCropped / duration;
			if (fadeFunction != nullptr) actualPercent = fadeFunction(actualPercent);

			Trans actualTrans(0, 0);
			actualTrans.localPos = from.localPos + (to.localPos - from.localPos)*actualPercent;
			actualTrans.rotation = from.rotation + (to.rotation - from.rotation)*actualPercent;
			actualTrans.scale = from.scale + (to.scale - from.scale)*actualPercent;
			actualTrans.CalcAbsTransform(owner->GetParent()->GetTransform());
			owner->SetTransform(actualTrans);

			if (actual >= (duration + delayAfter)) Finish();
		}

	};

}// namespace