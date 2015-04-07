
#ifndef ENBOUNDS_H
#define ENBOUNDS_H

#include "GNode.h"

class EnBounds{
private:
	float _width;
	float _height;

public:

	EnBounds(float width, float height);

	float GetWidth();

	void SetWidth(float val);

	float GetHeight();

	void SetHeight(float val);

	bool Collides(GNode* first, GNode* second);

};


EnBounds::EnBounds(float width, float height) : _width(width), _height(height){

	}

float EnBounds::GetWidth(){
		return _width;
	}

void EnBounds::SetWidth(float val){
		_width = val;
	}

float EnBounds::GetHeight(){
		return _height;
	}

void EnBounds::SetHeight(float val){
		_height = val;
	}

bool EnBounds::Collides(GNode* first, GNode* second){
	return false;
}

#endif