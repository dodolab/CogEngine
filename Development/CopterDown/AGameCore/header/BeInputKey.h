#ifndef BEINPUTKEY_H
#define BEINPUTKEY_H

#include "GBehavior.h"
#include "s3eKeyboard.h"
#include "MGameEngine.h"

/**
* Behavior for mapping key inputs to ACTIONS
*/
class BeInputKey : public GBehavior{
private:
	Act GetAction(s3eKey key){
		if (key == s3eKeyLeft) return Act::LEFT;
		if (key == s3eKeyRight) return Act::RIGHT;
		if (key == s3eKeySpace) return Act::FIRE;
		if (key == s3eKeyTab) return Act::SWITCH;
		return Act::NONE;
	}


public:
	BeInputKey() : GBehavior(ElemType::MODEL, EnFlags()){

	}

	virtual void OnMessage(Msg& msg){

	}

	virtual void Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner){

		if (owner->HasAttr(Attrs::ALLOWED_ACTIONS)){

			if (owner->HasAttr(Attrs::ACTIONS)){
				owner->AddAttr(Attrs::ACTIONS, EnFlags());
			}

			EnFlags& actions = owner->GetAttr<EnFlags>(Attrs::ACTIONS);
			EnFlags& allowedActions = owner->GetAttr<EnFlags>(Attrs::ALLOWED_ACTIONS);

			for (auto key : MEngine.environmentCtrl->GetPressedKeys()){

				if (!key.IsHandled() || key.handlerId == owner->GetId()){
					key.handlerId = owner->GetId();

					Act inAct = GetAction(key.key);
					if (inAct != Act::NONE && allowedActions.HasState((int)inAct)){
						if (!key.ended) actions.SetState((int)inAct);
						else actions.ResetState((int)inAct);
					}
				}
			}
		}
	}
};




#endif