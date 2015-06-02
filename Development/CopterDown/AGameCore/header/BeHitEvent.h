#ifndef BE_HITEVENT_H
#define BE_HITEVENT_H

#include "ABehavior.h"
#include "MGameEngine.h"
#include "EnHitImage.h"
#include "IwGeomFMat2D.h"
#include "BeTranslateAnim.h"

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

	void SwitchScene(GNode* owner){

		GNode* root = owner->GetRoot();
		GNode* first;
		GNode* second;
		GNode* third;

		for (auto it = root->GetChildren().begin(); it != root->GetChildren().end(); ++it){
			GNode* nod = (*it);

			if (nod->GetSubType() == 1) first = nod;
			else if (nod->GetSubType() == 2) second = nod;
			else third = nod;
		}

		int index = owner->GetSceneRoot()->GetSubType();

		if (index == 1){
			third->GetTransform().LocalPos.x = MEngine.environmentCtrl->GetWidth() * 2;
			BeTranslateAnim* trans = new BeTranslateAnim(CIwFVec2(MEngine.environmentCtrl->GetWidth() * 2, 0), CIwFVec2(MEngine.environmentCtrl->GetWidth(), 0), 1, false, false);
			third->AddBehavior(trans);

			trans = new BeTranslateAnim(CIwFVec2(first->GetTransform().LocalPos.x, 0), CIwFVec2(-MEngine.environmentCtrl->GetWidth(), 0), 1, false, false);
			first->AddBehavior(trans);
			trans = new BeTranslateAnim(CIwFVec2(MEngine.environmentCtrl->GetWidth(), 0), CIwFVec2(0, 0), 1, false, false);
			second->AddBehavior(trans);
		}
		else if (index == 2){
			first->GetTransform().LocalPos.x = MEngine.environmentCtrl->GetWidth() * 2;
			BeTranslateAnim* trans = new BeTranslateAnim(CIwFVec2(MEngine.environmentCtrl->GetWidth() * 2, 0), CIwFVec2(MEngine.environmentCtrl->GetWidth(), 0), 1, false, false);
			first->AddBehavior(trans);

			trans = new BeTranslateAnim(CIwFVec2(second->GetTransform().LocalPos.x, 0), CIwFVec2(-MEngine.environmentCtrl->GetWidth(), 0), 1, false, false);
			second->AddBehavior(trans);
			trans = new BeTranslateAnim(CIwFVec2(MEngine.environmentCtrl->GetWidth(), 0), CIwFVec2(0, 0), 1, false, false);
			third->AddBehavior(trans);
		}
		else{
			second->GetTransform().LocalPos.x = MEngine.environmentCtrl->GetWidth() * 2;
			BeTranslateAnim* trans = new BeTranslateAnim(CIwFVec2(MEngine.environmentCtrl->GetWidth() * 2, 0), CIwFVec2(MEngine.environmentCtrl->GetWidth(), 0), 1, false, false);
			second->AddBehavior(trans);

			trans = new BeTranslateAnim(CIwFVec2(third->GetTransform().LocalPos.x, 0), CIwFVec2(-MEngine.environmentCtrl->GetWidth(), 0), 1, false, false);
			third->AddBehavior(trans);
			trans = new BeTranslateAnim(CIwFVec2(MEngine.environmentCtrl->GetWidth(), 0), CIwFVec2(0, 0), 1, false, false);
			first->AddBehavior(trans);
		}
	}

	virtual void Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner){

		if (owner->HasState(States::HITTABLE)){

			if (owner->HasAttr(Attrs::HIT_IMAGE)){
				spt<EnHitImage> hitImage = owner->GetAttr<spt<EnHitImage>>(Attrs::HIT_IMAGE);

				for (auto touch : MEngine.environmentCtrl->GetPressedPoints()){

					if (touch.started){
						CIwFVec2 touchVector = touch.position;
						CIwFVec2 touchTrans = absMatrix.GetInverse().TransformVec(touchVector);


						if (hitImage->HitTest(touchTrans)){
							SwitchScene(owner);
							// is hit

							// initial
						/*	if (!owner->HasState(100) && !owner->HasState(101)) owner->SetState(101);

							// takovej pokus
							if (owner->HasState(100)){
								owner->SwitchState(100, 101);
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

							*/
						}
					}
				}
			}
			else{

				for (auto touch : MEngine.environmentCtrl->GetPressedPoints()){
					if (touch.started){
						CIwFVec2 touchVector = touch.position;
						CIwFVec2 touchTrans = absMatrix.GetInverse().TransformVec(touchVector);
						CIwFVec2 size = owner->GetAttr<CIwFVec2>(Attrs::SIZE);
						CIwFVec2 sizeTrans = owner->GetTransform().Scale*size;

						if (touchTrans.x+sizeTrans.x/2 <= sizeTrans.x && touchTrans.y+sizeTrans.y/2 <= sizeTrans.y &&
							touchTrans.x+sizeTrans.x/2 >= 0 && touchTrans.y+sizeTrans.y/2>=0){
							SwitchScene(owner);

						}
					}
				}
			}
		}
	}
};


#endif
