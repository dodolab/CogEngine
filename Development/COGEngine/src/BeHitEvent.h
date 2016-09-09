#ifndef BE_HITEVENT_H
#define BE_HITEVENT_H

#include "GBehavior.h"
#include "MGameEngine.h"
#include "BeTranslateAnim.h"
#include "Enums.h"

/**
* Behavior for hit testing
*/
class BeHitEvent : public GBehavior{
private:
	int handlerId;


public:
	BeHitEvent() : GBehavior(ElemType::MODEL, EnFlags()){
		handlerId = -1;
	}

	BeHitEvent(int handlerId) : GBehavior(ElemType::MODEL, EnFlags()), handlerId(handlerId){

	}

	virtual void OnMessage(GMsg& msg){

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

	virtual void Update(const uint64 delta, const uint64 absolute, const ofMatrix4x4& absMatrix, GNode* owner){

		if (owner->HasState(States::HITTABLE)){

			if (owner->HasAttr(Attrs::IMGSOURCE)){
				spt<ofImage> hitImage = owner->GetAttr<spt<ofImage>>(Attrs::IMGSOURCE);

				for (auto touch : MEngine.environmentCtrl->GetPressedPoints()){

					if (touch.started){
						ofVec3f touchVector = touch.position;
						ofVec3f touchTrans = touchVector*absMatrix.getInverse();

						if (ImageHitTest(hitImage, touchTrans)){ 
							// is hit
							//if (handlerId != -1){
								SendMessageNoResp(Traverses::BEH_FIRST, Actions::OBJECT_HIT, nullptr, owner);
							//}
						}
					}
				}
			}
			else{

				for (auto touch : MEngine.environmentCtrl->GetPressedPoints()){
					if (touch.started){
						ofVec3f touchVector = touch.position;
						ofVec3f touchTrans = absMatrix.getInverse()*(touchVector);
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


#endif
