#ifndef BE_HITEVENT_H
#define BE_HITEVENT_H

#include "ABehavior.h"
#include "MGameEngine.h"
#include "EnHitImage.h"
#include "IwGeomFMat2D.h"

/**
* Behavior for hit testing
*/
class BeHitEvent : public ABehavior{
private:



public:
	BeHitEvent() : ABehavior(ElemType::MODEL, EnFlags()){

	}

	virtual void OnMessage(Msg& msg){

	}

	virtual void Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner){
		
		if (owner->HasState(States::HITTABLE)){

			spt<EnHitImage> hitImage = owner->GetAttr<spt<EnHitImage>>(Attrs::HIT_IMAGE);

			for (auto touch : MEngine.environmentCtrl->GetPressedPoints()){

				if (touch.ended){
					CIwFVec2 touchVector = touch.position;
					CIwFVec2 touchTrans = absMatrix.GetInverse().TransformVec(touchVector);

				
					if (hitImage->HitTest(touchTrans)){
						// is hit

						// initial
						if (!owner->HasState(100) && !owner->HasState(101)) owner->SetState(100);

						// takovej pokus
						if (owner->HasState(100)){
							owner->SwitchState(100,101);
							if (owner->GetTag() == "fofka"){
								spt<CIw2DImage> image = MEngine.resourceCtrl->Get2DImage("images/rytmus.png");
								owner->ChangeAttr(Attrs::IMGSOURCE, image);
							}
							else{
								spt<CIw2DImage> image = MEngine.resourceCtrl->Get2DImage("images/blue.png");
								owner->ChangeAttr(Attrs::IMGSOURCE, image);
							}
							
						}
						else{
							owner->SwitchState(101, 100);
							if (owner->GetTag() == "fofka"){
								spt<CIw2DImage> image = MEngine.resourceCtrl->Get2DImage("images/rytmussmile.png");
								owner->ChangeAttr(Attrs::IMGSOURCE, image);
							}
							else{
								spt<CIw2DImage> image = MEngine.resourceCtrl->Get2DImage("images/red.png");
								owner->ChangeAttr(Attrs::IMGSOURCE, image);
							}
						}


					}
				}
			}

		}
	}
};


#endif
