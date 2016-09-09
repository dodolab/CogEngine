#ifndef BE_HITEVENT_H
#define BE_HITEVENT_H

#include "GBehavior.h"
#include "MGameEngine.h"
#include "EnHitImage.h"
#include "IwGeomFMat2D.h"
#include "BeTranslateAnim.h"
#include "Enums.h"

typedef  void(*HitHandler)(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner);

/**
* Behavior for hit testing
*/
class BeHitEvent : public GBehavior{
private:
	HitHandler handler = nullptr;


public:
	BeHitEvent() : GBehavior(ElemType::MODEL, EnFlags()){

	}

	BeHitEvent(HitHandler hitHandler) : GBehavior(ElemType::MODEL, EnFlags()){
		this->handler = hitHandler;
	}

	virtual void OnMessage(GMsg& msg){

	}

	virtual void Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner){

		if (owner->HasState(States::HITTABLE)){

			if (owner->HasAttr(Attrs::HIT_IMAGE)){
				spt<EnHitImage> hitImage = owner->GetAttr<spt<EnHitImage>>(Attrs::HIT_IMAGE);

				for (auto touch : MEngine.environmentCtrl->GetPressedPoints()){

					if (touch.started){
						Vectorf2 touchVector = touch.position;
						Vectorf2 touchTrans = absMatrix.GetInverse().TransformVec(touchVector);


						if (hitImage->HitTest(touchTrans)){
							// is hit
							if (handler != nullptr) handler(delta, absolute, absMatrix, owner);
						}
					}
				}
			}
			else{

				for (auto touch : MEngine.environmentCtrl->GetPressedPoints()){
					if (touch.started){
						Vectorf2 touchVector = touch.position;
						Vectorf2 touchTrans = absMatrix.GetInverse().TransformVec(touchVector);
						Vectorf2 size = owner->GetAttr<Vectorf2>(Attrs::SIZE);
						Vectorf2 sizeTrans = owner->GetTransform().Scale*size;

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
