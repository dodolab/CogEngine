#pragma once

#include "GBehavior.h"
#include "GNode.h"
#include "SmartPointer.h"
#include "EnPolygon.h"
#include "CopterFactory.h"

/**
* Behavior for copter live counter
*/
class CopterLives : public GBehavior{
private:
	int lives;


public:
	CopterLives(int lives) : GBehavior(ElemType::MODEL){
		this->lives = lives;
	}

	void Init(){
		RegisterListening(Actions::PARA_GROUNDED);
	}

	virtual void OnMessage(GMsg& msg){
		if (msg.GetAction() == Actions::PARA_GROUNDED){
			lives--;

			if (lives != 0){
				owner->ChangeAttr(Attrs::TEXT, string("LIVES: "+ofToString(lives)));
			}
			else{
				// death!!!
				SendMessage(Traversation(ScopeType::ROOT, true, true), Actions::PLAYER_LOOSE, nullptr, msg.GetSourceObject());
			}
		}
	}


	virtual void Update(const uint64 delta, const uint64 absolute){

	}
	
};
