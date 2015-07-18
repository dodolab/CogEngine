#pragma once

#include "GNode.h"
#include "Enums.h"
#include "ofBaseTypes.h"
#include "BeTempRender.h"
#include "BeDelayRemove.h"
#include "CopterFactory.h"

class CopterHelicopter : public GBehavior{

private:
	int frameCounter;
	int lastParaSpawn;

	CopterFactory* factory;

public:
	CopterHelicopter(CopterFactory* factory) : GBehavior(ElemType::MODEL){
		frameCounter = 0;
		lastParaSpawn = 0;
		this->factory = factory;
	}

	void OnMessage(GMsg& msg){
		if (msg.GetSourceObject()->GetId() == owner->GetId() && msg.GetAction() == Actions::HEALTH_CHANGED){
			int health = msg.GetSourceObject()->GetAttr<int>(Attrs::HEALTH);

			// add collision effect
			msg.GetSourceObject()->AddBehavior(new BeTempRender(COGGet2DImage("images/explosion.png"), 3));

			// remove copter with delay
			if (health < 0){
				msg.GetSourceObject()->AddBehavior(new BeDelayRemove(100));
				SendMessage(Traversation(ScopeType::SCENE, true, true), Actions::COPTER_KILLED, nullptr, msg.GetSourceObject());
			}
		}
	}

	void Init(){
		RegisterListening(Actions::HEALTH_CHANGED);
	}

	void Update(const uint64 delta, const uint64 absolute){

		
		ofVec2f velocity = owner->GetAttr<ofVec2f>(Attrs::VELOCITY);

		EnTransform& trans = owner->GetTransform();

		trans.LocalPos.x += velocity.x;
		trans.LocalPos.y += velocity.y;

		if (owner->HasState(States::TO_RIGHT) && velocity.x > 0 && owner->GetTransform().AbsPos.x > COGGetWidth() + 400){
			// go from right to left
			owner->SetState(States::TO_LEFT);
			owner->GetTransform().Scale.x = abs(owner->GetTransform().Scale.x);
			velocity.x = -abs(velocity.x);
			owner->ChangeAttr(Attrs::VELOCITY, velocity);
		}
		else if (owner->HasState(States::TO_LEFT) && velocity.x < 0 && owner->GetTransform().AbsPos.x < -200){
			// go from left to right
			owner->SetState(States::TO_RIGHT);
			owner->GetTransform().Scale.x = -abs(owner->GetTransform().Scale.x);
			velocity.x = abs(velocity.x);
			owner->ChangeAttr(Attrs::VELOCITY, velocity);
		}

		if (frameCounter % 5 == 0){
			// switch frame
			int frame = owner->GetAttr<int>(Attrs::FRAME);
			if (frame == 0){
				owner->ChangeAttr<int>(Attrs::FRAME, 1);
				spt<ofImage> img = COGGet2DImage("images/copter1.png");
				owner->ChangeAttr<spt<ofImage>>(Attrs::IMGSOURCE, img);
			}
			else{
				owner->ChangeAttr<int>(Attrs::FRAME, 0);
				spt<ofImage> img = COGGet2DImage("images/copter2.png");
				owner->ChangeAttr<spt<ofImage>>(Attrs::IMGSOURCE, img);
			}
		}

		// spawn per 8 sec
		GNode* scoreInfo = COGFindGameObjectByTag("score");
		int score = scoreInfo->HasAttr(Attrs::SCORE) ? scoreInfo->GetAttr<int>(Attrs::SCORE) : 0;

		int maximumPara = score == 0 ? 3 : 3*(1 + log10(score)*log10(score / 10));


		if (COGGetGameObjectsCountByTag("para") <= maximumPara && (absolute - lastParaSpawn) > (8000) && ((int)ofRandom(0,20)) == 10){
			float width = COGGetWidth();

			if (ofRandom(0, 2) == 1){
				// some randomness
				lastParaSpawn = absolute;
			}
			else{

				if (trans.AbsPos.x > 0.1f*width && trans.AbsPos.x < 0.9f*width && !(trans.AbsPos.x > 0.4f*width && trans.AbsPos.x < 0.6f*width)){
					// spawn para
					lastParaSpawn = absolute;
					owner->GetSceneRoot()->AddChild(factory->CreatePara(owner));
				}
			}
		}


		frameCounter++;
	}


};

