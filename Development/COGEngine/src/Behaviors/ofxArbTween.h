#pragma once

#include "ofxAreBehavior.h"

/*! Tween direction ofxArbTween scenes */
enum class TweenDirection{
	LEFT, RIGHT, UP, DOWN
};

/**x
* Behavior for scene tweening
*/
class ofxArbTween : public ofxAreBehavior{
protected:
	// scene that will be tweened out
	ofxAreNode* tweenOut;
	// scene that will be tweened in
	ofxAreNode* tweenIn;
	// tweening speed (in display width per second)
	float speed;
	// actual tween position
	float actual;

public:

	/**
	* Creates a new behavior for tweening animation ofxArbTween scenes
	* @param tweenIn scene that will be tweened in
	* @param tweenOut scene that will be tweened out
	* @param speed tweening speed
	*/
	ofxArbTween(ofxAreNode* tweenIn, ofxAreNode* tweenOut, float speed) : tweenIn(tweenIn), tweenOut(tweenOut), speed(speed), actual(0){

	}
};

/**
* Behavior for slide tweening 
*/
class BeSlideTween : public ofxArbTween{
private:
	// tween direction
	TweenDirection direction;
	
public:

	/**
	* Creates a new behavior for slide tweening
	* @param direction tween direction
	* @param tweenIn scene that will be tweened in
	* @param tweenOut scene that will be tweened out
	* @param speed tweening speed
	*/
	BeSlideTween(TweenDirection direction, ofxAreNode* tweenIn, ofxAreNode* tweenOut, float speed) : ofxArbTween(tweenIn, tweenOut, speed), direction(direction){

	}


	virtual void Update(const uint64 delta, const uint64 absolute){

			int width = COGGetScreenWidth();
			int height = COGGetScreenHeight();

			// calculate actual position
			actual += 1.0f / width * COGTranslateSpeed(speed)*delta;
			
			if (actual > 1){
				actual = 1;
			}

			// add sinus tweening
			float actSinus = (float)sin(actual*PI/2);

			float widthActual = width*actSinus;
			float heightActual = height*actSinus;

			// change position according to the tweening direction
			if (direction == TweenDirection::RIGHT){
				tweenOut->GetTransform().localPos.x = width / 2 + (widthActual);
				tweenIn->GetTransform().localPos.x = -width / 2 + widthActual;
			}
			else if (direction == TweenDirection::LEFT){
				tweenIn->GetTransform().localPos.x = width / 2 + (width - widthActual);
				tweenOut->GetTransform().localPos.x = width / 2 - widthActual;
			}
			else if (direction == TweenDirection::UP){
				tweenIn->GetTransform().localPos.x = width / 2;
				tweenIn->GetTransform().localPos.y = height / 2 + (height - heightActual);
				tweenOut->GetTransform().localPos.y = height / 2 - heightActual;
			}
			else if (direction == TweenDirection::DOWN){
				tweenIn->GetTransform().localPos.x = width / 2;
				tweenOut->GetTransform().localPos.y = height / 2 + (heightActual);
				tweenIn->GetTransform().localPos.y = -height / 2 + heightActual;
			}

			if (actual >= 1.0f){
				Finish();
				SendMessageNoBubbling(Actions::TWEEN_ENDED, 0, nullptr, tweenIn);
			}
	}
};
