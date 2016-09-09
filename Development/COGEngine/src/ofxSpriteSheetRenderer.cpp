#include "ofxSpriteSheetRenderer.h"

ofxSpriteSheetRenderer::ofxSpriteSheetRenderer(int _numLayers, int _tilesPerLayer, int _defaultLayer, int _tileSize)
{
	texture = nullptr;
	verts = nullptr;
	coords = nullptr;
	colors = nullptr;
	numSprites = nullptr;

	textureIsExternal = false;
	safeMode = true;

	reAllocateArrays(_numLayers, _tilesPerLayer, _defaultLayer, _tileSize);

	//shape stuff
	brushIndex = -1;
}

ofxSpriteSheetRenderer::~ofxSpriteSheetRenderer()
{
	if (texture != nullptr){
		if(textureIsExternal) texture->clear();
		else delete texture;
	}

	if (verts != nullptr)
		delete[] verts;
	if (coords != nullptr)
		delete[] coords;
	if (colors != nullptr)
		delete[] colors;
	if (numSprites != nullptr)
		delete[] numSprites;
}

void ofxSpriteSheetRenderer::reAllocateArrays(int _numLayers, int _tilesPerLayer, int _defaultLayer, int _tileSize)
{
	numLayers = _numLayers;
	tileSize = _tileSize;
	tilesPerLayer = _tilesPerLayer;
	defaultLayer = _defaultLayer;

	if (verts != nullptr)
		delete[] verts;
	if (coords != nullptr)
		delete[] coords;
	if (colors != nullptr)
		delete[] colors;
	if (numSprites != nullptr)
		delete[] numSprites;

	verts = new float[numLayers*tilesPerLayer * 18];
	coords = new float[numLayers*tilesPerLayer * 12];
	colors = new unsigned char[numLayers*tilesPerLayer * 24];
	numSprites = new int[numLayers];

	clearCounters();
	clearTexture();
}

void ofxSpriteSheetRenderer::clearTexture()
{
	if (texture != nullptr)
	{
		if (textureIsExternal)
			texture = nullptr; // just remove the refernece
		else
		{
			delete texture;
			texture = nullptr;
		}
	}
}

void ofxSpriteSheetRenderer::allocate(int widthHeight, int internalGLScaleMode)
{
	if (texture == nullptr)
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
	clearCounters();
	allocate(widthHeight, internalGLScaleMode);
	addMisc(fileName, 0, 0);
	finishTexture();
	textureIsExternal = false;
}

void ofxSpriteSheetRenderer::loadTexture(ofTexture * _texture, bool isExternal)
{
	clearCounters();
	clearTexture();
	textureIsExternal = isExternal;
	texture = _texture;
	spriteSheetWidth = _texture->getWidth() / tileSize;
	tileSize_f = tileSize;
#ifdef TARGET_OPENGLES	// if we don't have arb, it's crazy important that things are power of 2 so that this float is set properly
	tileSize_f /= widthHeight;
#endif

}

void ofxSpriteSheetRenderer::loadTexture(CollageTexture * _texture){
	loadTexture((ofTexture *)_texture);
}

void ofxSpriteSheetRenderer::loadTexture(SpriteTexture * _texture){
	loadTexture((ofTexture *)_texture);
}


void ofxSpriteSheetRenderer::clearCounters(int layer)
{
	if (layer == -1)
		for (int i = 0; i < numLayers; i++) numSprites[i] = 0;
	else
		numSprites[layer] = 0;
}

void ofxSpriteSheetRenderer::makeQuad(int offset, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4){
	verts[offset] = x1;
	verts[offset + 1] = y1;
	verts[offset + 2] = z1;

	verts[offset + 3] = verts[offset + 9] = x2;
	verts[offset + 4] = verts[offset + 10] = y2;
	verts[offset + 5] = verts[offset + 11] = z2;

	verts[offset + 6] = verts[offset + 12] = x3;
	verts[offset + 7] = verts[offset + 13] = y3;
	verts[offset + 8] = verts[offset + 14] = z3;

	verts[offset + 15] = x4;
	verts[offset + 16] = y4;
	verts[offset + 17] = z4;
}

void ofxSpriteSheetRenderer::makeColorQuad(int offset, int a, int r, int g, int b){
	colors[offset] = colors[offset + 4] = colors[offset + 8] = colors[offset + 12] = colors[offset + 16] = colors[offset + 20] = r;
	colors[offset + 1] = colors[offset + 5] = colors[offset + 9] = colors[offset + 13] = colors[offset + 17] = colors[offset + 21] = g;
	colors[offset + 2] = colors[offset + 6] = colors[offset + 10] = colors[offset + 14] = colors[offset + 18] = colors[offset + 22] = b;
	colors[offset + 3] = colors[offset + 7] = colors[offset + 11] = colors[offset + 15] = colors[offset + 19] = colors[offset + 23] = a;
}

void ofxSpriteSheetRenderer::addTexCoords(int offset, float x1, float y1, float x2, float y2){
	coords[offset] =     coords[offset + 4] = coords[offset + 8] = x1;
	coords[offset + 1] = coords[offset + 3] = coords[offset + 7] = y1;
	coords[offset + 2] = coords[offset + 6] = coords[offset + 10] = x2;
	coords[offset + 5] = coords[offset + 9] = coords[offset + 11] = y2;
}

bool ofxSpriteSheetRenderer::addTile(int index, int frame, float x, float y, float z,int layer, float w, float h, 
									 flipDirection f, float scale, float rot, int r, int g, int b, int alpha)
{
	if (layer == -1)
		layer = defaultLayer;

	if (texture == nullptr)
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
	int layerOffset = getLayerOffset(layer);
	int vertexOffset = getVertexOffset(layer);
	int colorOffset = getColorOffset(layer);

	getFrameXandY(index, frameX, frameY);

	ofPoint framePos = getFramePosOnSheet(frameX, frameY, frame, w);

	addTexCoords(f, framePos.x, framePos.y, layer, w, h);

	w = w*scale*tileSize/2;

	h*= h*scale*tileSize/2;

	//verticies ------------------------------------
	makeQuad(vertexOffset, 
		x + getX(-w, -h, rot), y + getY(-w, -h, rot),z,
		x + getX(w, -h, rot),  y + getY(w, -h, rot),z,
		x + getX(-w, h, rot),  y + getY(-w, h, rot),z,
		x + getX(w, h, rot),   y + getY(w, h, rot),z);



	//colors ---------------------------------------
	makeColorQuad(colorOffset, alpha,r,g,b);
	//----------------------------------------------
	numSprites[layer]++;

	return true;
}


void ofxSpriteSheetRenderer::draw()
{
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
	for (int i = 0; i <= numLayers - 1; i++){
		if (numSprites[i] > 0) glDrawArrays(GL_TRIANGLES, i*tilesPerLayer * 6, numSprites[i] * 6);
	}
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
	int layerOffset = getLayerOffset(layer);
	int coordOffset = getCoordOffset(layer);

	w *= tileSize_f;
	h *= tileSize_f;

	switch (f) {
	case F_NONE:
		addTexCoords(coordOffset, frameX, frameY, frameX+w, frameY+h);
		break;
	case F_HORIZ:
		addTexCoords(coordOffset, frameX+w, frameY, frameX, frameY+h);
		break;
	case F_VERT:
		addTexCoords(coordOffset, frameX, frameY+h, frameX+w, frameY);
		break;
	case F_HORIZ_VERT:
		addTexCoords(coordOffset, frameX+w, frameY+h, frameX, frameY);
		break;
	default:
		break;
	}
}


bool ofxSpriteSheetRenderer::addRect(float x, float y, float z, float w, float h, float scale, float rot, int r, int g, int b, int alpha, int layer)
{
	if (layer == -1)
		layer = defaultLayer;

	if (texture == nullptr)
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

	int layerOffset = getLayerOffset(layer);
	int vertexOffset = getVertexOffset(layer);
	int colorOffset = getColorOffset(layer);
	int coordOffset = getCoordOffset(layer);

	w = w*scale/2;
	h = h*scale/2;

	//verticies ------------------------------------
	makeQuad(vertexOffset, x + getX(-w, -h, rot), y + getY(-w, -h, rot),z,
		x + getX(w, -h, rot), y + getY(w, -h, rot),z,
		x + getX(-w, h, rot), y + getY(-w, h, rot),z,
		x + getX(w, h, rot),y + getY(w, h, rot),z);



	//colors ---------------------------------------
	makeColorQuad(colorOffset, alpha,r,g,b);

	//texture --------------------------------------
	addTexCoords(coordOffset, brushX+halfBrushSize, brushY+halfBrushSize,brushX+halfBrushSize, brushY+halfBrushSize);

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
	framePos.set(_frameX, _frameY);
	int rowNum = floor(_frame / spriteSheetWidth);
	framePos.x += (_frame - spriteSheetWidth * rowNum) * _w * tileSize_f;
	framePos.y += tileSize_f * rowNum;
	return framePos;
}

float ofxSpriteSheetRenderer::getX(int x, int y, float angle){
	if(angle == 0) return x;
	return (float)x*cosf(angle) - (float)y*sinf(angle);
}
float ofxSpriteSheetRenderer::getY(int x, int y, float angle){
	if(angle == 0) return y;
	return (float)x*sinf(angle) + (float)y*cosf(angle);
}

void ofxSpriteSheetRenderer::setBrushIndex(int index, int wh)
{
	brushIndex = index;
	getFrameXandY(brushIndex, brushX, brushY);

	brushSize = tileSize_f * wh;
	halfBrushSize = brushSize / 2;
}
