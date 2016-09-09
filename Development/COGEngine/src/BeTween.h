#pragma once

#include "GBehavior.h"

enum class TweenDirection{
	LEFT, RIGHT, UP, DOWN
};

/**
* Behavior for scene tweening
*/
class BeTween : public GBehavior{
protected:
	bool tweenStarted;
	bool tweenEnded ;
	GNode* tweenOut;
	GNode* tweenIn;
	float speed;

public:
	BeTween(GNode* tweenIn, GNode* tweenOut, float speed) : GBehavior(ElemType::MODEL), tweenIn(tweenIn), tweenOut(tweenOut), speed(speed){
		tweenStarted = false;
		tweenEnded = false;
	}
};

class BeSlideTween : public BeTween{
private:
	TweenDirection direction;
	float actual ;

public:
	BeSlideTween(TweenDirection direction, GNode* tweenIn, GNode* tweenOut, float speed) : BeTween(tweenIn, tweenOut, speed), direction(direction){
		actual = 0;
	}

	void Init(){
		
	}


	virtual void Update(const uint64 delta, const uint64 absolute){

		if (!tweenEnded){
			if (!tweenStarted){
				tweenStarted = true;
				return;
			}
			int width = COGGetWidth();
			int height = COGGetHeight();

			actual += 1.0f / width * COGTranslateSpeed(speed)*delta;
			
			if (actual > 1){
				actual = 1;
			}

			float widthActual = width*actual;
			float heightActual = height*actual;

			if (direction == TweenDirection::RIGHT){
				tweenOut->GetTransform().LocalPos.x = width / 2 + (widthActual);
				tweenIn->GetTransform().LocalPos.x = -width / 2  + widthActual;
			}
			else if (direction == TweenDirection::LEFT){
				tweenIn->GetTransform().LocalPos.x = width / 2 + (width - widthActual);
				tweenOut->GetTransform().LocalPos.x = width / 2 - widthActual;
			}
			else if (direction == TweenDirection::UP){
				tweenIn->GetTransform().LocalPos.x = width/2;
				tweenIn->GetTransform().LocalPos.y = height / 2 + (height - heightActual);
				tweenOut->GetTransform().LocalPos.y = height / 2 - heightActual;
			}
			else if (direction == TweenDirection::DOWN){
				tweenIn->GetTransform().LocalPos.x = width/2;
				tweenOut->GetTransform().LocalPos.y = height / 2 + (heightActual);
				tweenIn->GetTransform().LocalPos.y = -height / 2 + heightActual;
			}

			if (actual >= 1.0f){
				tweenEnded = true;
				SendMessage(Traversation(ScopeType::DIRECT_NO_TRAVERSE, true, true), Actions::TWEEN_ENDED, nullptr, tweenIn);
				owner->RemoveBehavior(this, false, true);
			}
		}
	}
};
