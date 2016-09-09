#pragma once

#include "ofxAreBehavior.h"
#include "ofxAraFlags.h"
#include "ofxAraBounds.h"
#include "ofxAraCollision.h"
#include "ofxAraShape.h"

/**x
* Behavior that switches button on-click images
*/
class ofxArbButton : public ofxAreBehavior{
protected:
	spt<ofImage> defaultImg;
	spt<ofImage> pressedImg;

public:

	/**
	* Creates a new switch-button behavior
	* @param defaultImg default image
	* @param pressedImg pressed image
	*/
	ofxArbButton(spt<ofImage> defaultImg, spt<ofImage> pressedImg) : 
		defaultImg(defaultImg), pressedImg(pressedImg){

	}

	void Init(){
		RegisterListening(Actions::OBJECT_HIT_STARTED, Actions::OBJECT_HIT_LOST, Actions::OBJECT_HIT_ENDED);
	}

	void OnMessage(ofxAreMsg& msg){
		if (msg.GetAction() == Actions::OBJECT_HIT_STARTED){
			if (msg.GetSourceObject()->GetId() == owner->GetId()){
				msg.GetSourceObject()->GetShape<spt<EnImageShape>>()->SetImage(pressedImg);
			}
		}

		if (msg.GetAction() == Actions::OBJECT_HIT_ENDED || msg.GetAction() == Actions::OBJECT_HIT_LOST){
			if (msg.GetSourceObject()->GetId() == owner->GetId()){
				msg.GetSourceObject()->GetShape<spt<EnImageShape>>()->SetImage(defaultImg);
			}
		}
	}


	void Update(const uint64 delta, const uint64 absolute){

	}

};


