
#ifndef ENBOUNDS_H
#define ENBOUNDS_H

#include "GNode.h"

class EnBounds{
private:
	float _width;
	float _height;

public:

	EnBounds(float width, float height);

	float GetWidth() const;

	void SetWidth(float val);

	float GetHeight() const;

	void SetHeight(float val);

	bool Collides(GNode& first, GNode& second) const;

};


EnBounds::EnBounds(float width, float height) : _width(width), _height(height){

	}

float EnBounds::GetWidth() const{
		return _width;
	}

void EnBounds::SetWidth(float val){
		_width = val;
	}

float EnBounds::GetHeight() const{
		return _height;
	}

void EnBounds::SetHeight(float val){
		_height = val;
	}

bool EnBounds::Collides(GNode& first, GNode& second) const{
	return false;
}

#endif