#ifndef COPTER_SCORE_H
#define COPTER_SCORE_H

#include "GBehavior.h"
#include "GNode.h"
#include "SmartPointer.h"
#include "EnPolygon.h"
#include "CopterFactory.h"

/**
* Behavior for copter score counter
*/
class CopterScore : public GBehavior{
private:
	int scoreDiff;

public:
	CopterScore() : GBehavior(ElemType::MODEL, EnFlags(Actions::PARA_KILLED, Actions::COPTER_KILLED)){
		scoreDiff = 0;
	}

	virtual void OnMessage(GMsg& msg){
		if (msg.GetAction() == Actions::PARA_KILLED){
			scoreDiff = 10;
		}
		else if (msg.GetAction() == Actions::COPTER_KILLED){
			scoreDiff = 100;
		}
	}


	virtual void Update(const uint64 delta, const uint64 absolute, GNode* owner){
		if (!owner->HasAttr(Attrs::SCORE)) owner->AddAttr(Attrs::SCORE, 0);

		int score = owner->GetAttr<int>(Attrs::SCORE);
		if (scoreDiff != 0){
			owner->ChangeAttr(Attrs::SCORE, score + scoreDiff);
			score += scoreDiff;
			scoreDiff = 0;
		}

		owner->ChangeAttr(Attrs::TEXT, string("SCORE: " + ofToString(score)));
	}
	
};


#endif