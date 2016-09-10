#pragma once

#include "Behavior.h"
#include "TransformEnt.h"
#include "EasingFunc.h"

namespace Cog {

	enum class AnimBlend {
		ADDITIVE, OVERLAY
	};

	/**x
	* Behavior for common transformation animation
	*/
	class TransformAnim : public Behavior {
	private:
		// duration in ms
		float duration = 0;
		// starting transform
		spt<TransformEnt> to = spt<TransformEnt>();
		Trans toTrans;
		// ending transform
		spt<TransformEnt> from = spt<TransformEnt>();
		Trans fromTrans;
		// actual duration
		float actual = 0;
		// fade function
		FadeFunction fadeFunction = nullptr;
		float delayAfter = 0;
		bool repeat = false;
		AnimBlend blend;

		float lastPercent = 0;

	public:

		TransformAnim() {

		}

		TransformAnim(spt<TransformEnt> from, spt<TransformEnt> to, float duration, float delayAfter, bool repeat, AnimBlend blend) : to(to), from(from), duration(duration),
			delayAfter(delayAfter), repeat(repeat), blend(blend) {
		}

		TransformAnim(spt<TransformEnt> from, spt<TransformEnt> to, float duration, float delayAfter, bool repeat, AnimBlend blend, FadeFunction fadeFunction) :
			to(to), from(from), duration(duration), delayAfter(delayAfter), repeat(repeat), blend(blend), fadeFunction(fadeFunction) {
		}

		virtual void Load(Setting& setting);

		void OnStart();

		void Update(const uint64 delta, const uint64 absolute);

		private:
			AnimBlend StrToAnimBlend(string val);
	};

}// namespace