#pragma once

#include "GBehavior.h"
#include "GNode.h"
#include "MEnums.h"

/**x
* Behavior for animation sequence; can store a collection of animations
*/
class BeMultiAnim : public GBehavior{

private:
	GBehavior* actual;
	vector<GBehavior*> animations;

protected:
	~BeMultiAnim(){
		// delete all behaviors
		for (auto it = animations.begin(); it != animations.end(); ++it){
			delete (*it);
		}
	}
public:

	/**
	* Creates a new behavior for animation sequence
	*/
	BeMultiAnim() : GBehavior(ElemType::MODEL){
		actual = nullptr;
	}

	void AddAnimation(GBehavior* anim){
		animations.push_back(anim);
		if (actual == nullptr) actual = anim;
	}

	void Init(){
		RegisterListening(Actions::BEHAVIOR_FINISHED);
		if (actual != nullptr)actual->Init();
		//SetOwner(actual, owner);
	}

	void OnMessage(GMsg& msg){
		
		if (actual != nullptr) actual->OnMessage(msg);
		
		if (msg.GetAction() == Actions::BEHAVIOR_FINISHED && msg.GetBehaviorId() == actual->GetId()){
			// get next behavior
			auto it = std::find(animations.begin(), animations.end(), actual);

			if ((it+1) != animations.end()){
				actual = *(++it); 
				actual->Init();
				//SetOwner(actual, owner);
			}
			else Finish();
		}
	}

	void Update(const uint64 delta, const uint64 absolute){
		
		if (actual != nullptr) actual->Update(delta, absolute);
	}

};

