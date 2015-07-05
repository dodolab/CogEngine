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
	int score;


public:
	CopterScore() : GBehavior(ElemType::MODEL, EnFlags(Actions::PARA_KILLED, Actions::COPTER_KILLED)){
		this->score = 0;
	}

	virtual void OnMessage(GMsg& msg){
		if (msg.GetAction() == Actions::PARA_KILLED){
			score += 10;
		}
		else if (msg.GetAction() == Actions::COPTER_KILLED){
			score += 100;
		}
	}


	virtual void Update(const uint64 delta, const uint64 absolute, GNode* owner){
		owner->ChangeAttr(Attrs::TEXT, string("SCORE: " + ofToString(score)));
	}
	
};


#endif