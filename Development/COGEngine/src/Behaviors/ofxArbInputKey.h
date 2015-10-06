#pragma once

#include "ofxAreBehavior.h"

/**x
* Behavior for mapping key inputs to ACTIONS
*/
class ofxArbInputKey : public ofxAreBehavior{
private:
	map<int, Act> keyMapping;

public:
	/**
	* Creates a new behavior that maps keys to actions
	* @param keyMapping key-to-action map
	*/
	ofxArbInputKey(map<int, Act> keyMapping) : keyMapping(keyMapping){

	}

	void Init(){
		if (!owner->HasAttr(Attrs::ACTIONS)){
			owner->AddAttr(Attrs::ACTIONS, ofxAraFlags());
		}
	}

	virtual void Update(const uint64 delta, const uint64 absolute){

		ofxAraFlags& actions = owner->GetAttr<ofxAraFlags>(Attrs::ACTIONS);

		for (auto key : COGGetPressedKeys()){

			if (!key.IsHandled() || key.handlerId == owner->GetId()){
				// handle key press
				key.handlerId = owner->GetId();

				Act inAct = GetAction(key.key);
				if (inAct != Act::NONE)
				{
					if (!key.ended) actions.SetState((int)inAct);
					else actions.ResetState((int)inAct);
				}
			}
		}
	}

private:

	// Gets action by key
	Act GetAction(int key){
		auto find = keyMapping.find(key);
		if (find != keyMapping.end()) return (*find).second;

		return Act::NONE;
	}
};

