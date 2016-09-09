#ifndef BE_HITEVENT_H
#define BE_HITEVENT_H

#include "GBehavior.h"
#include "MGameEngine.h"
#include "BeTranslateAnim.h"
#include "Enums.h"

typedef  void(*HitHandler)(const uint64 delta, const uint64 absolute, const ofMatrix4x4& absMatrix, GNode* owner);

/**
* Behavior for hit testing
*/
class BeHitEvent : public GBehavior{
private:
	HitHandler handler;


public:
	BeHitEvent() : GBehavior(ElemType::MODEL, EnFlags()){
		handler = nullptr;
	}

	BeHitEvent(HitHandler hitHandler) : GBehavior(ElemType::MODEL, EnFlags()){
		this->handler = hitHandler;
	}

	virtual void OnMessage(GMsg& msg){

	}

	bool ImageHitTest(spt<ofImage> image, Vectorf3 testPos){
		//Move the test position into "local" coordinate space
		Vectorf3 localPos = testPos + Vectorf3(image->width / 2, image->height / 2);
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
						Vectorf3 touchVector = touch.position;
						Vectorf3 touchTrans = touchVector*absMatrix.getInverse();

						if(ImageHitTest(hitImage, touchTrans)){
							// is hit
							if (handler != nullptr) handler(delta, absolute, absMatrix, owner);
						}
					}
				}
			}
			else{

				for (auto touch : MEngine.environmentCtrl->GetPressedPoints()){
					if (touch.started){
						Vectorf3 touchVector = touch.position;
						Vectorf3 touchTrans = absMatrix.getInverse()*(touchVector);
						Vectorf3 size = owner->GetAttr<Vectorf3>(Attrs::SIZE);
						Vectorf3 sizeTrans = owner->GetTransform().Scale*size;

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
