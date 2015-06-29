#ifndef BE_TWEEN_H
#define BE_TWEEN_H

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
	BeTween(GNode* tweenIn, GNode* tweenOut, float speed) : GBehavior(ElemType::MODEL, EnFlags()), tweenIn(tweenIn), tweenOut(tweenOut), speed(speed){
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
		// black background

		tweenStarted = true;
	}

	virtual void OnMessage(GMsg& msg){

	}

	virtual void Update(const uint64 delta, const uint64 absolute, const ofMatrix4x4& absMatrix, GNode* owner){
		if (!tweenEnded){
			if (!tweenStarted) Init();

			int width = MEngine.environmentCtrl->GetWidth();

			actual += 1.0f / width * MEngine.TranslateSpeed(speed)*delta;
			if (actual > 1) actual = 1;

			float widthActual = width*actual;

			tweenIn->GetTransform().LocalPos.x = width/2 + (width-widthActual);
			tweenOut->GetTransform().LocalPos.x = width / 2 - widthActual;
		}
	}
};


#endif
