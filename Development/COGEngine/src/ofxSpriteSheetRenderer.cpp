/***********************************************************************

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

#include "ofxSpriteSheetRenderer.h"

ofxSpriteSheetRenderer::ofxSpriteSheetRenderer(int _numLayers, int _tilesPerLayer, int _defaultLayer, int _tileSize)
{
	texture = NULL;
	verts = NULL;
	coords = NULL;
	colors = NULL;
	numSprites = NULL;

	textureIsExternal = false;

	safeMode = true;

	gameTime = ofGetElapsedTimeMillis();

	reAllocateArrays(_numLayers, _tilesPerLayer, _defaultLayer, _tileSize);

	//shape stuff
	brushIndex = -1;
}

ofxSpriteSheetRenderer::~ofxSpriteSheetRenderer()
{
	if (texture != NULL && textureIsExternal)
		texture->clear();

	if (verts != NULL)
		delete[] verts;
	if (coords != NULL)
		delete[] coords;
	if (colors != NULL)
		delete[] colors;
	if (numSprites != NULL)
		delete[] numSprites;
}

void ofxSpriteSheetRenderer::reAllocateArrays(int _numLayers, int _tilesPerLayer, int _defaultLayer, int _tileSize)
{
	numLayers = _numLayers;
	tileSize = _tileSize;
	tilesPerLayer = _tilesPerLayer;
	defaultLayer = _defaultLayer;

	if (verts != NULL)
		delete[] verts;
	if (coords != NULL)
		delete[] coords;
	if (colors != NULL)
		delete[] colors;
	if (numSprites != NULL)
		delete[] numSprites;

	verts = new float[numLayers*tilesPerLayer * 18];
	coords = new float[numLayers*tilesPerLayer * 12];
	colors = new unsigned char[numLayers*tilesPerLayer * 24];
	numSprites = new int[numLayers];

	clear();
	clearTexture();
}

void ofxSpriteSheetRenderer::clearTexture()
{
	if (texture != NULL)
	{
		if (textureIsExternal)
			texture = NULL; // just remove the refernece
		else
		{
			delete texture;
			texture = NULL;
		}
	}
}

void ofxSpriteSheetRenderer::allocate(int widthHeight, int internalGLScaleMode)
{
	if (texture == NULL)
	{
		tileSize_f = tileSize;
#ifdef TARGET_OPENGLES	// if we don't have arb, it's crazy important that things are power of 2 so that this float is set properly
		tileSize_f /= widthHeight;
#endif

		spriteSheetWidth = widthHeight / tileSize;

		CollageTexture * newTexture = new CollageTexture();

		newTexture->allocate(widthHeight, widthHeight, GL_RGBA, internalGLScaleMode);

		texture = (ofTexture*)newTexture;
	}
	else
		cerr << "cannot double allocate ofxSpriteSheetRenderer Texture! Please clearTexture() first" << endl;
}

void ofxSpriteSheetRenderer::addMisc(string fileName, int x, int y, int glType)
{
	CollageTexture *cTexture = dynamic_cast<CollageTexture*>(texture);
	cTexture->pasteImage(x, y, fileName, glType);
}

void ofxSpriteSheetRenderer::finishTexture()
{
	CollageTexture *cTexture = dynamic_cast<CollageTexture*>(texture);
	cTexture->finish();
}

void ofxSpriteSheetRenderer::loadTexture(string fileName, int widthHeight, int internalGLScaleMode)
{
	clearTexture();
	clear();
	allocate(widthHeight, internalGLScaleMode);
	addMisc(fileName, 0, 0);
	finishTexture();
	textureIsExternal = false;
}

void ofxSpriteSheetRenderer::loadTexture(ofTexture * _texture, bool isExternal)
{
	clear();
	clearTexture();
	textureIsExternal = isExternal;
	texture = _texture;
}

void ofxSpriteSheetRenderer::loadTexture(CollageTexture * _texture){
	loadTexture((ofTexture *)_texture);
}

void ofxSpriteSheetRenderer::loadTexture(PixelTexture * _texture){
	loadTexture((ofTexture *)_texture);
}

void ofxSpriteSheetRenderer::loadTexture(LinearTexture * _texture){
	loadTexture((ofTexture *)_texture);
}

void ofxSpriteSheetRenderer::clear(int layer)
{
	if (layer == -1)
		for (int i = 0; i < numLayers; i++) numSprites[i] = 0;
	else
		numSprites[layer] = 0;
}

bool ofxSpriteSheetRenderer::addTile(animation_t* sprite, float x, float y, float z, int layer, flipDirection f, float scale, int rot, int r, int g, int b, int alpha){
	int index, frame;

	if (layer == -1)
		layer = defaultLayer;

	// animation
	if (sprite->total_frames > 1)
		// still animating
		if (sprite->loops != 0)
			// time to advance frame
			if (gameTime > sprite->next_tick) {
				sprite->frame += sprite->frame_skip;
				// increment frame and keep it within range
				if (sprite->frame < 0) sprite->frame = sprite->total_frames - 1;
				if (sprite->frame >= sprite->total_frames) sprite->frame = 0;
				sprite->next_tick = gameTime + sprite->frame_duration;
				// decrement loop count if cycle complete
				if (((sprite->frame_skip > 0 && sprite->frame == sprite->total_frames - 1) || (sprite->frame_skip < 0 && sprite->frame == 0)) && sprite->loops > 0) sprite->loops--;
			}

	if (sprite->loops == 0 && sprite->final_index >= 0) {
		index = sprite->final_index;
		frame = 0;
	}
	else {
		index = sprite->index;
		frame = sprite->frame;
	}

	return addTile(index, frame, x, y, z,layer, sprite->w, sprite->h, f, scale, rot, r, g, b, alpha);
}


bool ofxSpriteSheetRenderer::addTile(int tile_name, int frame, float x, float y, float z,int layer, float w, float h, flipDirection f, float scale, int rot, int r, int g, int b, int alpha)
{
	if (layer == -1)
		layer = defaultLayer;

	if (texture == NULL)
	{
		cerr << "RENDER ERROR: No texture loaded!" << endl;
		return false;
	}

	if (numSprites[layer] >= tilesPerLayer)
	{
		cerr << "RENDER ERROR: Layer " << layer << " over allocated! Max " << tilesPerLayer << " sprites per layer!" << endl;
		return false;
	}

	if (layer > numLayers)
	{
		cerr << "RENDER ERROR: Bogus layer '" << layer << "'! Only " << numLayers << " layers compiled!" << endl;
		return false;
	}

	float frameX;
	float frameY;
	int layerOffset = layer*tilesPerLayer;
	int vertexOffset = (layerOffset + numSprites[layer]) * 18;
	int colorOffset = (layerOffset + numSprites[layer]) * 24;

	getFrameXandY(tile_name, frameX, frameY);

	ofPoint framePos = getFramePosOnSheet(frameX, frameY, frame, w);

	addTexCoords(f, framePos.x, framePos.y, layer, w, h);

	w *= scale*tileSize;
	w /= 2;

	h *= scale*tileSize;
	h /= 2;

	//verticies ------------------------------------
	verts[vertexOffset] = x + getX(-w, -h, rot);//x+scale*rotA->ul[rot  ]; //ul ur ll
	verts[vertexOffset + 1] = y + getY(-w, -h, rot);//y+scale*rotA->ul[rot+1];
	verts[vertexOffset + 2] = z;

	verts[vertexOffset + 3] = verts[vertexOffset + 9] = x + getX(w, -h, rot);//x+scale*rotA->ur[rot  ];
	verts[vertexOffset + 4] = verts[vertexOffset + 10] = y + getY(w, -h, rot);//y+scale*rotA->ur[rot+1];
	verts[vertexOffset + 5] = verts[vertexOffset + 11] = z;

	verts[vertexOffset + 6] = verts[vertexOffset + 12] = x + getX(-w, h, rot);//x+scale*rotA->ll[rot  ];
	verts[vertexOffset + 7] = verts[vertexOffset + 13] = y + getY(-w, h, rot);//y+scale*rotA->ll[rot+1];
	verts[vertexOffset + 8] = verts[vertexOffset + 14] = z;

	verts[vertexOffset + 15] = x + getX(w, h, rot);//x+scale*rotA->lr[rot  ];
	verts[vertexOffset + 16] = y + getY(w, h, rot);//y+scale*rotA->lr[rot+1];
	verts[vertexOffset + 17] = z;
	
	//colors ---------------------------------------

	colors[colorOffset] = colors[colorOffset + 4] = colors[colorOffset + 8] = colors[colorOffset + 12] = colors[colorOffset + 16] = colors[colorOffset + 20] = r;
	colors[colorOffset + 1] = colors[colorOffset + 5] = colors[colorOffset + 9] = colors[colorOffset + 13] = colors[colorOffset + 17] = colors[colorOffset + 21] = g;
	colors[colorOffset + 2] = colors[colorOffset + 6] = colors[colorOffset + 10] = colors[colorOffset + 14] = colors[colorOffset + 18] = colors[colorOffset + 22] = b;
	colors[colorOffset + 3] = colors[colorOffset + 7] = colors[colorOffset + 11] = colors[colorOffset + 15] = colors[colorOffset + 19] = colors[colorOffset + 23] = alpha;

	//----------------------------------------------
	numSprites[layer]++;

	return true;
}

void ofxSpriteSheetRenderer::update(unsigned long time)
{
	gameTime = time;
}

void ofxSpriteSheetRenderer::draw(int startLayer, int endLayer)
{
	if (endLayer == -1)
		endLayer = numLayers - 1;
	if (safeMode)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	glVertexPointer(3, GL_FLOAT, 0, &verts[0]);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, &colors[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &coords[0]);

	texture->bind();
	for (int i = startLayer; i <= endLayer; i++)
		if (numSprites[i] > 0)
			glDrawArrays(GL_TRIANGLES, i*tilesPerLayer * 6, numSprites[i] * 6);
	texture->unbind();

	if (safeMode)
	{
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

void ofxSpriteSheetRenderer::addTexCoords(flipDirection f, float &frameX, float &frameY, int layer, float w, float h)
{
	int layerOffset = layer*tilesPerLayer;
	int coordOffset = (layerOffset + numSprites[layer]) * 12;

	w *= tileSize_f;
	h *= tileSize_f;

	switch (f) {
	case F_NONE:
		coords[coordOffset] =     coords[coordOffset + 4] = coords[coordOffset + 8] = frameX;
		coords[coordOffset + 1] = coords[coordOffset + 3] = coords[coordOffset + 7] = frameY;
		coords[coordOffset + 2] = coords[coordOffset + 6] = coords[coordOffset + 10] = frameX + w;
		coords[coordOffset + 5] = coords[coordOffset + 9] = coords[coordOffset + 11] = frameY + h;

		break;
	case F_HORIZ:
		coords[coordOffset] =     coords[coordOffset + 4] = coords[coordOffset + 8] = frameX+w;
		coords[coordOffset + 1] = coords[coordOffset + 3] = coords[coordOffset + 7] = frameY;
		coords[coordOffset + 2] = coords[coordOffset + 6] = coords[coordOffset + 10] = frameX;
		coords[coordOffset + 5] = coords[coordOffset + 9] = coords[coordOffset + 11] = frameY + h;

		break;
	case F_VERT:
		coords[coordOffset] =     coords[coordOffset + 4] = coords[coordOffset + 8] = frameX;
		coords[coordOffset + 1] = coords[coordOffset + 3] = coords[coordOffset + 7] = frameY+h;
		coords[coordOffset + 2] = coords[coordOffset + 6] = coords[coordOffset + 10] = frameX+w;
		coords[coordOffset + 5] = coords[coordOffset + 9] = coords[coordOffset + 11] = frameY;

		break;
	case F_HORIZ_VERT:
		coords[coordOffset] =     coords[coordOffset + 4] = coords[coordOffset + 8] = frameX+w;
		coords[coordOffset + 1] = coords[coordOffset + 3] = coords[coordOffset + 7] = frameY + h;
		coords[coordOffset + 2] = coords[coordOffset + 6] = coords[coordOffset + 10] = frameX;
		coords[coordOffset + 5] = coords[coordOffset + 9] = coords[coordOffset + 11] = frameY;

		break;
	default:
		break;
	}
}


bool ofxSpriteSheetRenderer::addRect(float x, float y, float z, float w, float h, float scale, float rot, int r, int g, int b, int alpha, int layer)
{
	if (layer == -1)
		layer = defaultLayer;

	if (texture == NULL)
	{
		cerr << "RENDER ERROR: No texture loaded!" << endl;
		return false;
	}

	if (numSprites[layer] >= tilesPerLayer)
	{
		cerr << "RENDER ERROR: Layer " << layer << " over allocated! Max " << tilesPerLayer << " sprites per layer!" << endl;
		return false;
	}

	if (layer > numLayers)
	{
		cerr << "RENDER ERROR: Bogus layer '" << layer << "'! Only " << numLayers << " layers compiled!" << endl;
		return false;
	}

	if (brushIndex < 0)
	{
		cerr << "RENDER ERROR: brush not initialized" << endl;
		return false;
	}

	int layerOffset = layer*tilesPerLayer;
	int vertexOffset = (layerOffset + numSprites[layer]) * 18;
	int colorOffset = (layerOffset + numSprites[layer]) * 24;
	int coordOffset = (layerOffset + numSprites[layer]) * 12;

	w *= scale;
	w /= 2;

	h *= scale;
	h /= 2;

	//verticies ------------------------------------
	verts[vertexOffset] = x + getX(-w, -h, rot);//x+scale*rotA->ul[rot  ]; //ul ur ll
	verts[vertexOffset + 1] = y + getY(-w, -h, rot);//y+scale*rotA->ul[rot+1];
	verts[vertexOffset + 2] = 0;

	verts[vertexOffset + 3] = verts[vertexOffset + 9] = x + getX(w, -h, rot);//x+scale*rotA->ur[rot  ];
	verts[vertexOffset + 4] = verts[vertexOffset + 10] = y + getY(w, -h, rot);//y+scale*rotA->ur[rot+1];
	verts[vertexOffset + 5] = verts[vertexOffset + 11] = 0;

	verts[vertexOffset + 6] = verts[vertexOffset + 12] = x + getX(-w, h, rot);//x+scale*rotA->ll[rot  ];
	verts[vertexOffset + 7] = verts[vertexOffset + 13] = y + getY(-w, h, rot);//y+scale*rotA->ll[rot+1];
	verts[vertexOffset + 8] = verts[vertexOffset + 14] = 0;

	verts[vertexOffset + 15] = x + getX(w, h, rot);//x+scale*rotA->lr[rot  ];
	verts[vertexOffset + 16] = y + getY(w, h, rot);//y+scale*rotA->lr[rot+1];
	verts[vertexOffset + 17] = 0;

	//colors ---------------------------------------
	colors[colorOffset] = colors[colorOffset + 4] = colors[colorOffset + 8] = colors[colorOffset + 12] = colors[colorOffset + 16] = colors[colorOffset + 20] = r;
	colors[colorOffset + 1] = colors[colorOffset + 5] = colors[colorOffset + 9] = colors[colorOffset + 13] = colors[colorOffset + 17] = colors[colorOffset + 21] = g;
	colors[colorOffset + 2] = colors[colorOffset + 6] = colors[colorOffset + 10] = colors[colorOffset + 14] = colors[colorOffset + 18] = colors[colorOffset + 22] = b;
	colors[colorOffset + 3] = colors[colorOffset + 7] = colors[colorOffset + 11] = colors[colorOffset + 15] = colors[colorOffset + 19] = colors[colorOffset + 23] = alpha;

	//texture --------------------------------------

	coords[coordOffset] = coords[coordOffset + 2] = coords[coordOffset + 4] = coords[coordOffset + 6] = coords[coordOffset + 8] = coords[coordOffset + 10] = brushX + halfBrushSize;
	coords[coordOffset + 1] = coords[coordOffset + 3] = coords[coordOffset + 5] = coords[coordOffset + 7] = coords[coordOffset + 9] = coords[coordOffset + 11] = brushY + halfBrushSize;

	numSprites[layer]++; // sprite == 2 triangles

	return true;
}

void ofxSpriteSheetRenderer::getFrameXandY(int tile_position, float &x, float &y)
{

	y = (tile_position / spriteSheetWidth);
	x = (tile_position - y * spriteSheetWidth);

	x *= tileSize_f;
	y *= tileSize_f;
}

ofPoint ofxSpriteSheetRenderer::getFramePosOnSheet(float _frameX, float _frameY, int _frame, float _w) {

	ofPoint framePos;
	framePos.set(_frameX, _frameY); // These get passed in externally and are probably zero, but maybe not, so we'll use them.
	int rowNum = floor(_frame / spriteSheetWidth);
	framePos.x += (_frame - spriteSheetWidth * rowNum) * _w * tileSize_f;
	framePos.y += tileSize_f * rowNum;
	return framePos;
}

float ofxSpriteSheetRenderer::getX(int x, int y, float angle){
	return (float)x*cos(angle) - (float)y*sin(angle);
}
float ofxSpriteSheetRenderer::getY(int x, int y, float angle){
	return (float)x*sin(angle) + (float)y*cos(angle);
}

void ofxSpriteSheetRenderer::setBrushIndex(int index, int wh)
{
	brushIndex = index;
	getFrameXandY(brushIndex, brushX, brushY);

	brushSize = tileSize_f * wh;
	halfBrushSize = brushSize / 2;
}
