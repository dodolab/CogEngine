#pragma once

#include "Behavior.h"
#include "TransformEnt.h"
#include "EasingFunc.h"

namespace Cog {

	/**
	* Type of animation blending
	*/
	enum class AnimBlend {
		ADDITIVE,	/** values will be added with the current  */
		OVERLAY		/** values will be replaced*/
	};

	/**
	* Behavior for general transformation animation
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
		// delay after the animation finishes (in ms)
		float delayAfter = 0;
		// indicator whether the animation should be repeated
		bool repeat = false;
		// type of value blending
		AnimBlend blend;

		// percentage of the last update
		float lastPercent = 0;

	public:

		TransformAnim() {

		}

		/**
		* Creates a new Transform animation
		* @param from starting transform
		* @param to ending transform
		* @param duration duration of the transformation in ms
		* @param delayAfter the delay between animation and behavior finish (in ms)
		* @param repeat indicator whether the animation should be repeated
		* @param blend type of value blending
		*/
		TransformAnim(spt<TransformEnt> from, spt<TransformEnt> to, float duration, 
			float delayAfter, bool repeat, AnimBlend blend) 
			: to(to), from(from), duration(duration), delayAfter(delayAfter), repeat(repeat), blend(blend) {
		}

		/**
		* Creates a new Transform animation
		* @param from starting transform
		* @param to ending transform
		* @param duration duration of the transformation in ms
		* @param delayAfter the delay between animation and behavior finish (in ms)
		* @param repeat indicator whether the animation should be repeated
		* @param blend type of value blending
		* @param fadeFunction fading function
		*/
		TransformAnim(spt<TransformEnt> from, spt<TransformEnt> to, float duration, 
			float delayAfter, bool repeat, AnimBlend blend, FadeFunction fadeFunction) 
			: to(to), from(from), duration(duration), delayAfter(delayAfter), repeat(repeat), blend(blend), fadeFunction(fadeFunction) {
		}

		virtual void Load(Setting& setting);

		void OnStart();

		void Update(const uint64 delta, const uint64 absolute);

	};

}// namespace