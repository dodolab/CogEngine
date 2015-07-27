#pragma once

#include "GBehavior.h"
#include "MEnums.h"

#ifdef TARGET_ANDROID
#include "ofxAndroidVibrator.h"
#endif


/**
* Behavior for hit testing
*/
class BeHitEvent : public GBehavior{
protected:
	// if true, device will vibrate when object is hit
	bool vibrate;
	// indicator, if hit has started over this object
	bool hitStarted;
	// id of button who started the hit
	int hitStartedTouchId;
	// id of handler behavior (-1 for all behaviors)
	int handlerBehId;

public:

	/**
	* Creates a new behavior for hit testing
	* @param handlerBehId id of handler behavior (set -1 for all handlers that listens HIT events)
	* @param vibrate if true, device will vibrate when object is hit
	*/
	BeHitEvent(int handlerBehId, bool vibrate) : GBehavior(ElemType::MODEL), 
		handlerBehId(handlerBehId), vibrate(vibrate), hitStarted(false), hitStartedTouchId(-1){

	}

	void Init(){
		owner->SetState(States::HITTABLE);
	}

	/**
	* Tests if the image has been hit
	* @param image image to test
	* @param testPos test position
	*/
	bool ImageHitTest(spt<ofImage> image, ofVec3f testPos){
		// move the test position into "local" coordinate space
		ofVec3f localPos = testPos + ofVec3f(image->width / 2, image->height / 2);
		// test for location outside the image rectangle
		if (localPos.x < 0
			|| localPos.y < 0
			|| localPos.x >(float)image->width
			|| localPos.y >(float)image->height)
			return false;

		ofColor col = image->getColor(localPos.x, localPos.y);
		// return a hit if the specified local alpha value is greater than half
		return col.a > 0x80;
	}

	virtual void Update(const uint64 delta, const uint64 absolute){

		if (owner->HasState(States::HITTABLE)){

			// get inverse matrix
			ofMatrix4x4 inverse = owner->GetTransform().GetAbsMatrix().getInverse();

			if (owner->HasAttr(Attrs::IMGSOURCE)){
				spt<ofImage> hitImage = owner->GetAttr<spt<ofImage>>(Attrs::IMGSOURCE);

				bool atLeastOneTouch = false;

				for (auto touch : COGGetPressedPoints()){

					// calculate vector in image space
					ofVec3f touchVector = touch.position;
					ofVec3f touchTrans = touchVector*inverse;

					if (ImageHitTest(hitImage, touchTrans)){
						// image has been hit
						if (touch.started){
#ifdef TARGET_ANDROID
							if(vibrate) ofxAndroidVibrator::vibrate(50);
#endif
							atLeastOneTouch = true;
							hitStarted = true;
							hitStartedTouchId = touch.touchId;

							owner->SetState(States::HIT);
							if (handlerBehId == -1) SendMessageNoBubbling(Actions::OBJECT_HIT_STARTED, nullptr, owner);
							else SendDirectMessage(Actions::OBJECT_HIT_STARTED, nullptr, owner, handlerBehId);
						}
						else if (touch.ended){
							

							owner->ResetState(States::HIT);
							if (hitStarted){
								if (handlerBehId == -1) SendMessageNoBubbling(Actions::OBJECT_HIT_ENDED, nullptr, owner);
								else SendDirectMessage(Actions::OBJECT_HIT_ENDED, nullptr, owner, handlerBehId);
							}
							else{
								if (handlerBehId == -1) SendMessageNoBubbling(Actions::OBJECT_HIT_LOST, nullptr, owner);
								else SendDirectMessage(Actions::OBJECT_HIT_LOST, nullptr, owner, handlerBehId);
							}

							if (hitStartedTouchId == touch.touchId && hitStarted){
								hitStarted = false;
							}
						}
						else{
							atLeastOneTouch = true;
							if (!owner->HasState(States::HIT)){
#ifdef TARGET_ANDROID
								if(vibrate) ofxAndroidVibrator::vibrate(50);
#endif
								// touch hasn't started but this object hasn't been hit
								owner->SetState(States::HIT);
							}
							// hit started and continues
							if (hitStarted){
								// hit started, lost and started again
								if (handlerBehId == -1) SendMessageNoBubbling(Actions::OBJECT_HIT_STARTED, nullptr, owner);
								else SendDirectMessage(Actions::OBJECT_HIT_STARTED, nullptr, owner, handlerBehId);
							}
							else{
								// hit started but not on first touch
								if (handlerBehId == -1) SendMessageNoBubbling(Actions::OBJECT_HIT_OVER, nullptr, owner);
								else SendDirectMessage(Actions::OBJECT_HIT_OVER, nullptr, owner, handlerBehId);
							}
						}
					}
					else{
						if (touch.ended && touch.touchId == hitStartedTouchId){
							// object isn't hit and this hit has already ended
							hitStarted = false;
						}
					}
				}

				if (!atLeastOneTouch){
					// object could lost its hit
					if (owner->HasState(States::HIT)){
						owner->ResetState(States::HIT);
						if (handlerBehId == -1) SendMessageNoBubbling(Actions::OBJECT_HIT_LOST, nullptr, owner);
						else SendDirectMessage(Actions::OBJECT_HIT_LOST, nullptr, owner, handlerBehId);
					}
				}

			}
			else{
				// TODO
				/*for (auto touch : COGGetPressedPoints()){
					if (touch.started){
						ofVec3f touchVector = touch.position;
						ofVec3f touchTrans = inverse*(touchVector);
						ofVec3f size = owner->GetAttr<ofVec3f>(Attrs::SIZE);
						ofVec3f sizeTrans = owner->GetTransform().Scale*size;

						if (touchTrans.x+sizeTrans.x/2 <= sizeTrans.x && touchTrans.y+sizeTrans.y/2 <= sizeTrans.y &&
							touchTrans.x+sizeTrans.x/2 >= 0 && touchTrans.y+sizeTrans.y/2>=0){
							// is hit

						}
					}
				}*/
			}
		}
	}
};
