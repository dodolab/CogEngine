#ifndef BEINPUTKEY_H
#define BEINPUTKEY_H

#include "GBehavior.h"
#include "MGameEngine.h"

/**
* Behavior for mapping key inputs to ACTIONS
*/
class BeInputKey : public GBehavior{
private:
	Act GetAction(int key){
		if (key == OF_KEY_LEFT) return Act::LEFT;
		if (key == OF_KEY_RIGHT) return Act::RIGHT;
		if (key == OF_KEY_SHIFT) return Act::FIRE;
		if (key == OF_KEY_TAB) return Act::SWITCH;
		return Act::NONE;
	}


public:
	BeInputKey() : GBehavior(ElemType::MODEL, EnFlags()){

	}

	virtual void OnMessage(GMsg& msg){

	}

	virtual void Update(const uint64 delta, const uint64 absolute, GNode* owner){

	//	if (owner->HasAttr(Attrs::ALLOWED_ACTIONS)){

			if (!owner->HasAttr(Attrs::ACTIONS)){
				owner->AddAttr(Attrs::ACTIONS, EnFlags());
			}

			EnFlags& actions = owner->GetAttr<EnFlags>(Attrs::ACTIONS);
			//EnFlags& allowedActions = owner->GetAttr<EnFlags>(Attrs::ALLOWED_ACTIONS);

			for (auto key : MEngine.environmentCtrl->GetPressedKeys()){

				if (!key.IsHandled() || key.handlerId == owner->GetId()){
					key.handlerId = owner->GetId();

					Act inAct = GetAction(key.key);
					if (inAct != Act::NONE)
					{//&& allowedActions.HasState((int)inAct)){
						if (!key.ended) actions.SetState((int)inAct);
						else actions.ResetState((int)inAct);
					}
				}
			}
		}
	//}
};




#endif