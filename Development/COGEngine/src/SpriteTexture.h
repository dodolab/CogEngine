/*
 *  pixelTexture.h
 *  plsSendRenderer
 *
 *  Created by Zach Gage on 5/23/10.
 *  Copyright 2010 stfj. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

class SpriteTexture : public ofTexture
{
public:

	SpriteTexture(bool isPixel) : isPixel(isPixel){

	}



	void allocate(int w, int h, int internalGlDataType);
	void allocate(int w, int h, int internalGlDataType, bool bUseARBExtention);
	
	void loadTexture(string textureName, int glType=GL_RGBA);
	void loadImage(string textureName);
	
	// if true, NEAREST mode is used
	bool isPixel;

	int width;
	int height;

  struct {
		GLenum glType;
		GLenum pixelType;
	} internal;
};
