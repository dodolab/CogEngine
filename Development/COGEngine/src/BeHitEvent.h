#pragma once

#include "GBehavior.h"
#include "MGameEngine.h"
#include "BeTranslateAnim.h"
#include "Enums.h"
#include "GMsg.h"

#ifdef TARGET_ANDROID
#include "ofxAndroidVibrator.h"
#endif

/**
* Behavior for hit testing
*/
class BeHitEvent : public GBehavior{


public:
	BeHitEvent() : GBehavior(ElemType::MODEL, EnFlags()){

	}


	bool ImageHitTest(spt<ofImage> image, ofVec3f testPos){
		//Move the test position into "local" coordinate space
		ofVec3f localPos = testPos + ofVec3f(image->width / 2, image->height / 2);
		//Test for location outside the image rectangle
		if (localPos.x < 0
			|| localPos.y < 0
			|| localPos.x >(float)image->width
			|| localPos.y >(float)image->height)
			return false;

		ofColor col = image->getColor(localPos.x, localPos.y);
		//Return a hit if the specified local alpha value is greater than half
		return col.a > 0x80;
	}

	virtual void Update(const uint64 delta, const uint64 absolute){

		if (owner->HasState(States::HITTABLE)){

			ofMatrix4x4 inverse = owner->GetTransform().GetAbsMatrix().getInverse();


			if (owner->HasAttr(Attrs::IMGSOURCE)){
				spt<ofImage> hitImage = owner->GetAttr<spt<ofImage>>(Attrs::IMGSOURCE);

				bool atLeastOneTouch = false;

				for (auto touch : MEngine.environmentCtrl->GetPressedPoints()){

					ofVec3f touchVector = touch.position;
					ofVec3f touchTrans = touchVector*inverse;

					if (ImageHitTest(hitImage, touchTrans)){
						if (touch.started){
#ifdef TARGET_ANDROID
							    ofxAndroidVibrator::vibrate(50);
#endif
								atLeastOneTouch = true;
								owner->SetState(States::HIT);
								SendMessage(Traversation(ScopeType::DIRECT_NO_TRAVERSE, true, true), Actions::OBJECT_HIT, nullptr, owner);
						}
						else if (touch.ended){
							// todo: multitouch problem!!
								//owner->ResetState(States::HIT);
								//SendMessage(Traversation(ScopeType::DIRECT_NO_TRAVERSE, true, true), Actions::OBJECT_RELEASED, nullptr, owner);
						}
						else{
							atLeastOneTouch = true;
							if (!owner->HasState(States::HIT)){
#ifdef TARGET_ANDROID
							    ofxAndroidVibrator::vibrate(50);
#endif
								
								owner->SetState(States::HIT);
								SendMessage(Traversation(ScopeType::DIRECT_NO_TRAVERSE, true, true), Actions::OBJECT_HIT, nullptr, owner);
							}
						}
					}
					else{
						if (owner->HasState(States::HIT)){
							//owner->ResetState(States::HIT);
							//SendMessage(Traversation(ScopeType::DIRECT_NO_TRAVERSE, true, true), Actions::OBJECT_RELEASED, nullptr, owner);
						}
					}

					

				}

				if (!atLeastOneTouch){
					if (owner->HasState(States::HIT)){
						owner->ResetState(States::HIT);
						SendMessage(Traversation(ScopeType::DIRECT_NO_TRAVERSE, true, true), Actions::OBJECT_RELEASED, nullptr, owner);
					}
				}

			}
			else{

				for (auto touch : MEngine.environmentCtrl->GetPressedPoints()){
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
				}
			}
		}
	}
};
