#include "Tween.h"
#include "ofxCogEngine.h"
#include "Scene.h"
#include "Node.h"

namespace Cog {

	void OverrideTween::Update(const uint64 delta, const uint64 absolute) {

		if (to->GetRunningMode() == RUNMODE_INVISIBLE) {
			to->SetRunningMode(RUNMODE_RUNNING);
		}

		int width = CogGetVirtualWidth();
		int height = CogGetVirtualHeight();

		// calculate actual position
		actual += 1.0f / width * speed*0.001f*width*delta;

		if (actual > 1) {
			actual = 1;
		}

		float fadeValue;

		if (fadeFunction != nullptr) fadeValue = fadeFunction(actual);
		else {
			fadeValue = (float)sin(actual*PI / 2);
		}

		float widthActual = width*fadeValue;
		float heightActual = height*fadeValue;

		// change position according to the tweening direction
		if (direction == TWEEN_RIGHT) {
			to->GetScene()->GetViewPortOffset().x = -width + widthActual;
		}
		else if (direction == TWEEN_LEFT) {
			to->GetScene()->GetViewPortOffset().x = width - widthActual;
		}
		else if (direction == TWEEN_UP) {
			to->GetScene()->GetViewPortOffset().x = 0;
			to->GetScene()->GetViewPortOffset().y = (float)(height - heightActual);
		}
		else if (direction == TWEEN_DOWN) {
			to->GetScene()->GetViewPortOffset().x = (float)(0);
			to->GetScene()->GetViewPortOffset().y = (float)(-height + heightActual);
		}

		if (actual >= 1.0f) {
			Finish();
			SendMessage(ACT_TWEEN_ENDED, to);
		}
	}


	void SlideTween::Update(const uint64 delta, const uint64 absolute) {

		if (to->GetRunningMode() == RUNMODE_INVISIBLE) {
			to->SetRunningMode(RUNMODE_RUNNING);
		}

		int width = CogGetVirtualWidth();
		int height = CogGetVirtualHeight();

		// calculate actual position
		actual += 1.0f / width * speed*0.001f*width*delta;

		if (actual > 1) {
			actual = 1;
		}

		float fadeValue;

		if (fadeFunction != nullptr) fadeValue = fadeFunction(actual);
		else {
			fadeValue = (float)sin(actual*PI / 2);
		}

		float widthActual = width*fadeValue;
		float heightActual = height*fadeValue;

		// change position according to the tweening direction
		if (direction == TWEEN_RIGHT) {
			from->GetScene()->GetViewPortOffset().x = (widthActual);
			to->GetScene()->GetViewPortOffset().x = -width + widthActual;
		}
		else if (direction == TWEEN_LEFT) {
			from->GetScene()->GetViewPortOffset().x = (-widthActual);
			to->GetScene()->GetViewPortOffset().x = width - widthActual;
		}
		else if (direction == TWEEN_UP) {
			to->GetScene()->GetViewPortOffset().x = 0;
			from->GetScene()->GetViewPortOffset().y = (float)((-heightActual));
			to->GetScene()->GetViewPortOffset().y = (float)(height - heightActual);
		}
		else if (direction == TWEEN_DOWN) {
			to->GetScene()->GetViewPortOffset().x = (float)(0);
			from->GetScene()->GetViewPortOffset().y = (float)((heightActual));
			to->GetScene()->GetViewPortOffset().y = (float)(-height + heightActual);
		}

		if (actual >= 1.0f) {
			Finish();
			SendMessage(ACT_TWEEN_ENDED, to);
		}
	}

}// namespace