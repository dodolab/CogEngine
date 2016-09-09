#pragma once

#include "ofxAreBehavior.h"
#include "ofxAreMsg.h"
#include "ofxAreNode.h"

/**x
* Behavior for simple movement
*/
class ofxArbMove : public ofxAreBehavior{
protected:
	float speed;
public:

	/**
	* Creates a new behavior for simple movement
	* @param speed movement speed
	*/
	ofxArbMove(float speed) : speed(speed){

	}

	void Init(){
		if (!owner->HasAttr(Attrs::VELOCITY)){
			owner->AddAttr(Attrs::VELOCITY, ofVec3f());
		}
	}

	void Update(const uint64 delta, const uint64 absolute){
		ofxAraTransform& transform = owner->GetTransform();

		ofVec3f velocity = owner->GetAttr<ofVec3f>(Attrs::VELOCITY);

		transform.localPos.x += COGTranslateSpeed(speed)*velocity.x * delta;
		transform.localPos.y += COGTranslateSpeed(speed)*velocity.y * delta;
	}

};