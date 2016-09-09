/***********************************************************************

version 2.0

Copyright (C) 2011 by Zach Gage and Ramsey Nasser

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

************************************************************************/

/*
ofxSpriteSheetRender provides most of the methods you would want for bringing
in a sprite sheet, splitting it up, assigning animations, and drawing them
in a batched style.
*/

#ifndef OFX_SPRITE_SHEET_RENDERER_H
#define OFX_SPRITE_SHEET_RENDERER_H

#include "ofMain.h"
#include "CollageTexture.h"
#include "PixelTexture.h"
#include "LinearTexture.h"

typedef struct {
	int index;                    //								integer index into the sprite sheet to draw from. sprite sheet is numbered left to right, top to bottom
	int frame;                    // DEFAULT SHOULD BE SET TO 0.	which frame of the animation to draw. current tile is calculated as index+frame..
	int total_frames;             //								the total number of frames in the animation, or 1 if it is not animated

	float w;                      //								how many tiles right of the index are included
	float h;                      //								how many tiles down from the index are included

	unsigned int frame_duration;  //								how many milliseconds each frame should be on screen. less = faster animation.
	unsigned long next_tick;      // DEFAULT SHOULD BE SET TO 0.	when in gametime the frame should be changed. updated automatically.

	int loops;                    //								the number of times to loop the animation. -1 means forever, don't set it to 0.
	int final_index;              //								index of the tile to draw once animation is done. -1 means the default of total_frames-1. 
	int frame_skip;				 //	 DEFAULT SHOULD BE SET TO 1.	the inverval by which frames are moved between. Generally this should be 1, but could be set to -1 if you wanted the animation to play backwards for example
} animation_t;


extern unsigned long gameTime;

enum flipDirection { F_NONE = 0, F_HORIZ, F_VERT, F_HORIZ_VERT };

#define ANIMATION_MINIMUM_FRAME_DURATION 25
#define ANIMATION_MAXIMUM_FRAME_DURATION 1000

//class -------------------------------------------
class ofxSpriteSheetRenderer
{
public:
	int brushIndex;
	float brushX;
	float brushY;


	float brushSize;
	float halfBrushSize;

	bool safeMode;

	/**
	*
	* @param _numLayers number of layers
	* @param _tilesPerLayer layer capacity (how much tiles can be stored in one layer)
	* @param _defaultLayer default layer index (0)
	* @param _tileSize default size of one tile
	*/
	ofxSpriteSheetRenderer(int _numLayers, int _tilesPerLayer, int _defaultLayer, int _tileSize);
	~ofxSpriteSheetRenderer();

	void reAllocateArrays(int _numLayers, int _tilesPerLayer, int _defaultLayer, int _tileSize);

	/**
	* Loads texture from file
	* @param widthHeight - texture size (should be power of 2)
	* @param internalGLScaleMode - opengl scaling, possibles values: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST,
	* GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR
	*/
	void loadTexture(string fileName, int widthHeight, int internalGLScaleMode);
	void loadTexture(ofTexture * _texture, bool isExternal = true);
	void loadTexture(CollageTexture * _texture);
	void loadTexture(PixelTexture * _texture);
	void loadTexture(LinearTexture * _texture);

	// -----------------------------------------

	void setBrushIndex(int index, int wh = 1);

	//shapes (require a brushTexture to be on the sprite sheet)
	bool addRect(float x, float y, float z, float w, float h, float scale, float rot, int r = 255, int g = 255, int b = 255, int alpha = 255, int layer = -1);

	// -----------------------------------------

	void clear(int layer = -1);


	bool addTile(animation_t* sprite, float x, float y, float z, int layer = -1, flipDirection f = F_NONE, float scale = 1.0, int rot = 0, int r = 255, int g = 255, int b = 255, int alpha = 255); // this assumes the sprite is width height equal

	bool addTile(int tile_name, int frame, float x, float y, float z, int layer = -1, float w = 1, float h = 1, flipDirection f = F_NONE, float scale = 1.0, int rot = 0, int r = 255, int g = 255, int b = 255, int alpha = 255);

	void update(unsigned long time);
	void draw(int startLayer = 0, int endLayer = -1);

	int getSpriteSheetWidth(){
		return spriteSheetWidth;
	};

	int getTileSize(){
		return tileSize;
	};



	// texture creation ------------------------

	void allocate(int widthHeight, int internalGLScaleMode);
	void clearTexture();

	void addMisc(string fileName, int x, int y, int glType = GL_RGBA);

	void finishTexture();

	// -----------------------------------------

protected:

	float tileSize_f;
	bool textureIsExternal;

	ofTexture * texture;

	int numLayers;
	int defaultLayer;
	int tilesPerLayer;
	int tileSize;

	unsigned long gameTime;

	float * verts;
	float * coords;
	unsigned char * colors;

	int * numSprites;
	int spriteSheetWidth;

	void getFrameXandY(int tile_position, float &x, float &y);

	ofPoint getFramePosOnSheet(float _frameX, float _frameY, int _frame, float _w); // Added by J. Matthew Griffis to support multi-row animation. Returns the x and y of the frame as a point.

	float getX(int x, int y, float angle);
	float getY(int x, int y, float angle);

	void addTexCoords(flipDirection f, float &frameX, float &frameY, int layer, float x = 1, float y = 1);




};

#endif