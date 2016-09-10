
#include "TranslateAnim.h"
#include "CogEngine.h"

namespace Cog {


	TranslateAnim::TranslateAnim(ofVec3f from, ofVec3f to, float speed, bool additive) :
		additive(additive), to(to), from(from) {
		this->actual = ofVec3f(from);
		this->distX = to.x - from.x;
		this->distY = to.y - from.y;

		float distance = (float)sqrt(distX*distX + distY*distY);
		speedX = speed*distX / distance;
		speedY = speed*distY / distance;
		fadeFunction = nullptr;

	}

	TranslateAnim::TranslateAnim(ofVec3f from, ofVec3f to, float speed, FadeFunction fadeFunction) :
		additive(additive), to(to), from(from) {
		this->actual = ofVec3f(from);
		this->distX = to.x - from.x;
		this->distY = to.y - from.y;

		float distance = (float)sqrt(distX*distX + distY*distY);
		speedX = speed*distX / distance;
		speedY = speed*distY / distance;

		this->fadeFunction = fadeFunction;
		this->additive = false;
	}


	void TranslateAnim::Update(const uint64 delta, const uint64 absolute) {

		// calculate differencial
		float diffX = (float)(speedX*0.001*CogGetScreenWidth()*delta);
		float diffY = (float)(speedY*0.001*CogGetScreenWidth()*delta);
		bool finished = false;

		actual.x += diffX;
		actual.y += diffY;

		if (ofVec3f(from - to).lengthSquared() < ofVec3f(from - actual).lengthSquared())
		{
			actual = ofVec3f(to);
			finished = true;
		}

		Trans& transform = owner->GetTransform();

		// change position
		if (additive)
		{
			transform.localPos.x += diffX;
			transform.localPos.y += diffY;
		}
		else
		{
			float lengthFromTo = ofVec3f(from - to).length();
			float lengthFromActual = ofVec3f(from - actual).length();
			float percent = lengthFromActual / lengthFromTo;

			if (fadeFunction != nullptr) percent = fadeFunction(percent);

			ofVec3f actualAmount = (to - from)*(percent);
			transform.localPos = from + actualAmount;
		}

		if (finished) Finish();
	}


}// namespace