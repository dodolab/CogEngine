#ifndef BE_DELAY_REM_H
#define BE_DELAY_REM_H

#include "GBehavior.h"
#include "MGameEngine.h"
#include "BeTranslateAnim.h"
#include "Enums.h"
#include "GMsg.h"

/**
* Behavior that removes object with delay
*/
class BeDelayRemove : public GBehavior{
private:
	int millis;
	int actual;

public:
	BeDelayRemove(int millis) : GBehavior(ElemType::MODEL, EnFlags()){
		this->millis = millis;
		this->actual = 0;
	}


	virtual void OnMessage(GMsg& msg){

	}


	virtual void Update(const uint64 delta, const uint64 absolute, GNode* owner){
		actual += delta;

		if (actual > millis){
			owner->GetParent()->RemoveChild(owner,false,true);
		}
	}
};


#endif
