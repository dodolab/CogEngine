#pragma once

#include "CogBehavior.h"

/**x
* Behavior for mapping key inputs to ACTIONS
*/
class CogInputKey : public CogBehavior{
private:
	map<int, CogAct> keyMapping;

public:
	/**
	* Creates a new behavior that maps keys to actions
	* @param keyMapping key-to-action map
	*/
	CogInputKey(map<int, CogAct> keyMapping) : keyMapping(keyMapping){

	}

	void Init(){
		if (!owner->HasAttr(Attrs::ACTIONS)){
			owner->AddAttr(Attrs::ACTIONS, CogFlags());
		}
	}

	virtual void Update(const uint64 delta, const uint64 absolute){

		CogFlags& actions = owner->GetAttr<CogFlags>(Attrs::ACTIONS);

		for (auto key : CogGetPressedKeys()){

			if (!key.IsHandled() || key.handlerId == owner->GetId()){
				// handle key press
				key.handlerId = owner->GetId();

				CogAct inAct = GetAction(key.key);
				if (inAct != CogAct::NONE)
				{
					if (!key.ended) actions.SetState((int)inAct);
					else actions.ResetState((int)inAct);
				}
			}
		}
	}

private:

	// Gets action by key
	CogAct GetAction(int key){
		auto find = keyMapping.find(key);
		if (find != keyMapping.end()) return (*find).second;

		return CogAct::NONE;
	}
};

