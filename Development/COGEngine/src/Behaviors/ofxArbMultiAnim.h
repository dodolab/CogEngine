#pragma once

#include "ofxAreBehavior.h"
#include "ofxAreNode.h"

/**x
* Behavior for animation sequence; can store a collection of animations
*/
class ofxArbMultiAnim : public ofxAreBehavior{

private:
	ofxAreBehavior* actual;
	vector<ofxAreBehavior*> animations;

public:

	/**
	* Creates a new behavior for animation sequence
	*/
	ofxArbMultiAnim(){
		actual = nullptr;
	}

	void AddAnimation(ofxAreBehavior* anim){
		animations.push_back(anim);
		if (actual == nullptr) actual = anim;
	}

	void Init(){
		RegisterListening(Actions::BEHAVIOR_FINISHED);
		if (actual != nullptr)actual->Init();
		//SetOwner(actual, owner);
	}

	void OnMessage(ofxAreMsg& msg){
		
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

protected:
	~ofxArbMultiAnim(){
		// delete all behaviors
		for (auto it = animations.begin(); it != animations.end(); ++it){
			delete (*it);
		}
	}
};

