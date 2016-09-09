#pragma once

#include "GBehavior.h"

/*! Tween direction between scenes */
enum class TweenDirection{
	LEFT, RIGHT, UP, DOWN
};

/**x
* Behavior for scene tweening
*/
class BeTween : public GBehavior{
protected:
	// scene that will be tweened out
	GNode* tweenOut;
	// scene that will be tweened in
	GNode* tweenIn;
	// tweening speed (in display width per second)
	float speed;
	// actual tween position
	float actual;

public:

	/**
	* Creates a new behavior for tweening animation between scenes
	* @param tweenIn scene that will be tweened in
	* @param tweenOut scene that will be tweened out
	* @param speed tweening speed
	*/
	BeTween(GNode* tweenIn, GNode* tweenOut, float speed) : GBehavior(ElemType::MODEL), tweenIn(tweenIn), tweenOut(tweenOut), speed(speed), actual(0){

	}
};

/**
* Behavior for slide tweening 
*/
class BeSlideTween : public BeTween{
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
	BeSlideTween(TweenDirection direction, GNode* tweenIn, GNode* tweenOut, float speed) : BeTween(tweenIn, tweenOut, speed), direction(direction){

	}


	virtual void Update(const uint64 delta, const uint64 absolute){

			int width = COGGetWidth();
			int height = COGGetHeight();

			// calculate actual position
			actual += 1.0f / width * COGTranslateSpeed(speed)*delta;
			
			if (actual > 1){
				actual = 1;
			}

			float widthActual = width*actual;
			float heightActual = height*actual;

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
				SendMessage(BubblingType(ScopeType::DIRECT_NO_TRAVERSE, true, true), Actions::TWEEN_ENDED, nullptr, tweenIn);
			}
	}
};
