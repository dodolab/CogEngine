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
	bool tweenStarted = false;
	bool tweenEnded = false;
	GNode* tweenOut;
	GNode* tweenIn;
	float speed;

public:
	BeTween(GNode* tweenIn, GNode* tweenOut, float speed) : GBehavior(ElemType::MODEL, EnFlags()), tweenIn(tweenIn), tweenOut(tweenOut), speed(speed){

	}
};

class BeSlideTween : public BeTween{
private:
	TweenDirection direction;
	float actual = 0;
	
public:
	BeSlideTween(TweenDirection direction, GNode* tweenIn, GNode* tweenOut, float speed) : BeTween(tweenIn, tweenOut, speed), direction(direction){

	}

	void Init(){
		// black background

		tweenStarted = true;
	}

	virtual void OnMessage(GMsg& msg){

	}

	virtual void Update(const uint64 delta, const uint64 absolute, const CIwFMat2D& absMatrix, GNode* owner){
		if (!tweenEnded){
			if (!tweenStarted) Init();

			int width = MEngine.environmentCtrl->GetWidth();

			actual += speed / 50.0f;
			if (actual > 1) actual = 1;

			float widthActual = width*actual;

			tweenIn->GetTransform().LocalPos.x = width/2 + (width-widthActual);
			tweenOut->GetTransform().LocalPos.x = width / 2 - widthActual;
		}
	}
};


#endif
