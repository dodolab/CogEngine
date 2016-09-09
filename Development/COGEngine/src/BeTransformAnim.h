#pragma once

#include "GBehavior.h"
#include "GNode.h"
#include "MEnums.h"



/**x
* Behavior for common transformation animation
*/
class BeTransformAnim : public GBehavior{

private:
	// duration in ms
	float duration;
	// starting transform
	EnTransform to;
	// ending transform
	EnTransform from;
	// actual duration
	float actual;
	// fade function
	FadeFunction fadeFunction;
	float delayAfter;

public:

	BeTransformAnim(EnTransform from, EnTransform to, float duration, float delayAfter) : GBehavior(ElemType::MODEL), to(to), from(from), duration(duration), 
		delayAfter(delayAfter){
		this->actual = 0;
		fadeFunction = nullptr;
	}

	BeTransformAnim(EnTransform from, EnTransform to, float duration, float delayAfter, FadeFunction fadeFunction) : GBehavior(ElemType::MODEL), 
		to(to), from(from), duration(duration), delayAfter(delayAfter){
		this->actual = 0;
		this->fadeFunction = fadeFunction;
	}

	void Update(const uint64 delta, const uint64 absolute){
		
		actual += delta;

		float actualCropped = actual;

		if (actual >= duration){
			actualCropped = duration;
		}

		float actualPercent = actualCropped / duration;
		if (fadeFunction != nullptr) actualPercent = fadeFunction(actualPercent);

		EnTransform actualTrans(0, 0);
		actualTrans.localPos = from.localPos + (to.localPos - from.localPos)*actualPercent;
		actualTrans.rotation = from.rotation + (to.rotation - from.rotation)*actualPercent;
		actualTrans.scale = from.scale + (to.scale - from.scale)*actualPercent;
		actualTrans.CalcAbsTransform(owner->GetParent()->GetTransform());
		owner->SetTransform(actualTrans);

		if (actual >= (duration + delayAfter)) Finish();
	}

};

