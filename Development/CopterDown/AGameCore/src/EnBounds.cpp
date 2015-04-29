#include "EnBounds.h"

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

