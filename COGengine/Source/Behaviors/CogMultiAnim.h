#pragma once

#include "CogBehavior.h"
#include "CogNode.h"

/**x
* Behavior for animation sequence; can store a collection of animations
*/
class CogMultiAnim : public CogBehavior{

private:
	CogBehavior* actual;
	vector<CogBehavior*> animations;

public:

	/**
	* Creates a new behavior for animation sequence
	*/
	CogMultiAnim(){
		actual = nullptr;
	}

	void AddAnimation(CogBehavior* anim){
		animations.push_back(anim);
		if (actual == nullptr) actual = anim;
	}

	void Init(){
		RegisterListening(Actions::BEHAVIOR_FINISHED);
		if (actual != nullptr)actual->Init();
		SetOwner(actual, owner);
	}

	void OnMessage(CogMsg& msg){
		
		if (actual != nullptr) actual->OnMessage(msg);
		
		if (msg.GetAction() == Actions::BEHAVIOR_FINISHED && msg.GetBehaviorId() == actual->GetId()){
			// get next behavior
			auto it = std::find(animations.begin(), animations.end(), actual);

			if ((it+1) != animations.end()){
				actual = *(++it); 
				actual->Init();
				SetOwner(actual, owner);
			}
			else Finish();
		}
	}

	void Update(const uint64 delta, const uint64 absolute){
		
		if (actual != nullptr) actual->Update(delta, absolute);
	}

protected:
	~CogMultiAnim(){
		// delete all behaviors
		for (auto it = animations.begin(); it != animations.end(); ++it){
			delete (*it);
		}
	}
};

