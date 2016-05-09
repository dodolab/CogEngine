#pragma once

#include "TransformMath.h"
#include "Node.h"

class DummyShape : public Mesh {
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
