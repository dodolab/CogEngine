#pragma once

#include "TransformMath.h"

class DummyShape : public Shape {
public:
	int width;
	int height;

	DummyShape(int width, int height) : width(width), height(height) {

	}

	float GetWidth() const {
		return width;
	}

	float GetHeight() const {
		return height;
	}
};
