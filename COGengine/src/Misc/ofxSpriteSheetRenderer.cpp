#include "ofxSpriteSheetRenderer.h"

ofxSpriteSheetRenderer::ofxSpriteSheetRenderer()
{
	buffers = map<string, SpriteBuffer*>();
	actualBuffer = nullptr;
	safeMode = true;

	//shape stuff
	brushIndex = -1;
}

ofxSpriteSheetRenderer::~ofxSpriteSheetRenderer()
{
	for (auto it = buffers.begin(); it != buffers.end(); ++it){
		SpriteBuffer* buf = (*it).second;
		delete buf;
	}

}

void ofxSpriteSheetRenderer::reAllocateArrays(string sheetName, int bufferSize)
{
	setActualBuffer(sheetName);

	actualBuffer->bufferSize = bufferSize;


	if (actualBuffer->verts != nullptr)
		delete[] actualBuffer->verts;
	if (actualBuffer->coords != nullptr)
		delete[] actualBuffer->coords;
	if (actualBuffer->colors != nullptr)
		delete[] actualBuffer->colors;

	actualBuffer->verts = new float[actualBuffer->bufferSize * 18];
	actualBuffer->coords = new float[actualBuffer->bufferSize * 12];
	actualBuffer->colors = new unsigned char[actualBuffer->bufferSize * 24];
	actualBuffer->numSprites = 0;

	clearCounters(sheetName);
	clearTexture(sheetName);
}

void ofxSpriteSheetRenderer::clearTexture(string sheetName)
{
	setActualBuffer(sheetName);

	if (actualBuffer->texture != nullptr)
	{
		if (actualBuffer->textureIsExternal)
			actualBuffer->texture = nullptr; // just remove the refernece
		else
		{
			delete actualBuffer->texture;
			actualBuffer->texture = nullptr;
		}
	}
}

void ofxSpriteSheetRenderer::allocate(string sheetName, int textureWidth, int textureHeight, int internalGLScaleMode)
{
	setActualBuffer(sheetName);

	if (actualBuffer->texture == nullptr)
	{
		setTexCoeffs(textureWidth, textureHeight);

		CollageTexture * newTexture = new CollageTexture();

		newTexture->allocate(textureWidth, textureHeight, GL_RGBA, internalGLScaleMode);

		actualBuffer->texture = (ofTexture*)newTexture;
	}
	else
		cerr << "cannot double allocate ofxSpriteSheetRenderer Texture! Please clearTexture() first" << endl;
}

void ofxSpriteSheetRenderer::addMisc(string fileName, string sheetName, int x, int y, int glType)
{
	setActualBuffer(sheetName);

	CollageTexture *cTexture = dynamic_cast<CollageTexture*>(actualBuffer->texture);
	cTexture->pasteImage(x, y, fileName, glType);
}

void ofxSpriteSheetRenderer::finishTexture(string sheetName)
{
	setActualBuffer(sheetName);

	CollageTexture *cTexture = dynamic_cast<CollageTexture*>(actualBuffer->texture);
	cTexture->finish();
}

void ofxSpriteSheetRenderer::loadTexture(string fileName, string sheetName, int bufferSize, int width, int height, int internalGLScaleMode)
{
	if (buffers.find(sheetName) == buffers.end()){
		buffers[sheetName] = new SpriteBuffer();
	}

	reAllocateArrays(sheetName, bufferSize);
	allocate(sheetName, width, height, internalGLScaleMode);
	addMisc(fileName, sheetName, 0, 0);
	finishTexture(sheetName);
	actualBuffer->textureIsExternal = false;
	
}

void ofxSpriteSheetRenderer::loadTexture(ofTexture * texture, string sheetName, int bufferSize, bool isExternal)
{
	if (buffers.find(sheetName) == buffers.end()){
		buffers[sheetName] = new SpriteBuffer();
	}

	clearCounters(sheetName);
	clearTexture(sheetName);
	reAllocateArrays(sheetName, bufferSize);
	actualBuffer->textureIsExternal = isExternal;
	actualBuffer->texture = texture;

	setTexCoeffs(texture->getWidth(), texture->getHeight());

}

void ofxSpriteSheetRenderer::loadTexture(CollageTexture * _texture, string sheetName, int bufferSize){
	loadTexture((ofTexture *)_texture, sheetName, bufferSize);
}

void ofxSpriteSheetRenderer::loadTexture(SpriteTexture * _texture, string sheetName, int bufferSize){
	loadTexture((ofTexture *)_texture, sheetName, bufferSize);
}


void ofxSpriteSheetRenderer::clearCounters(string sheetName)
{
	setActualBuffer(sheetName);
	actualBuffer->numSprites = 0;
}

void ofxSpriteSheetRenderer::makeQuad(int offset, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4){
	
	float* verts = actualBuffer->verts;

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

void ofxSpriteSheetRenderer::makeQuad(int offset, Tile& tile){
	float w = tile.width*tile.scaleX / 2;
	float h = tile.height*tile.scaleY / 2;
	float x = tile.posX;
	float y = tile.posY;
	float z = tile.posZ;
	float rot = tile.rotation;

	float* verts = actualBuffer->verts;

	verts[offset + 2] = verts[offset + 5] = verts[offset + 11] = verts[offset + 8] = verts[offset + 14] = verts[offset + 17] = z;

	verts[offset] = x+getX(-w,-h,rot);
	verts[offset + 1] = y+getY(-w,-h,rot);
	verts[offset + 3] = verts[offset + 9] = x+getX(w,-h,rot);
	verts[offset + 4] = verts[offset + 10] = y+getY(w,-h,rot);
	verts[offset + 6] = verts[offset + 12] = x+getX(-w,h,rot);
	verts[offset + 7] = verts[offset + 13] = y+getY(-w,h,rot);
	verts[offset + 15] = x+getX(w,h,rot);
	verts[offset + 16] = y+getY(w,h,rot);
}

void ofxSpriteSheetRenderer::makeColorQuad(int offset, int a, int r, int g, int b){
	unsigned char* colors = actualBuffer->colors;

	colors[offset] = colors[offset + 4] = colors[offset + 8] = colors[offset + 12] = colors[offset + 16] = colors[offset + 20] = r;
	colors[offset + 1] = colors[offset + 5] = colors[offset + 9] = colors[offset + 13] = colors[offset + 17] = colors[offset + 21] = g;
	colors[offset + 2] = colors[offset + 6] = colors[offset + 10] = colors[offset + 14] = colors[offset + 18] = colors[offset + 22] = b;
	colors[offset + 3] = colors[offset + 7] = colors[offset + 11] = colors[offset + 15] = colors[offset + 19] = colors[offset + 23] = a;
}

void ofxSpriteSheetRenderer::addTexCoords(int offset, float x1, float y1, float x2, float y2){
	float* coords = actualBuffer->coords;

	coords[offset] =     coords[offset + 4] = coords[offset + 8] = x1;
	coords[offset + 1] = coords[offset + 3] = coords[offset + 7] = y1;
	coords[offset + 2] = coords[offset + 6] = coords[offset + 10] = x2;
	coords[offset + 5] = coords[offset + 9] = coords[offset + 11] = y2;
}

bool ofxSpriteSheetRenderer::addTile(Tile& tile)
{
	if (actualBuffer == nullptr){
		cerr << "RENDER ERROR: Sheet doesn't exist!" << endl;
		return false;
	}

	if (actualBuffer->texture == nullptr)
	{
		cerr << "RENDER ERROR: No texture loaded!" << endl;
		return false;
	}

	if (actualBuffer->numSprites >= actualBuffer->bufferSize)
	{
		cerr << "RENDER ERROR: Spritesheet over allocated! Max " << actualBuffer->bufferSize << " sprites!" << endl;
		return false;
	}

	int vertexOffset = getVertexOffset();
	int colorOffset = getColorOffset();
	
	//addTexCoords(tile.dir, tile.offsetX, tile.offsetY, layer, tile.width, tile.height);
	addTexCoords(tile);

	makeQuad(vertexOffset, tile);
	
	/*
	float w = tile.width*tile.scaleX/ 2;
	float h = tile.height*tile.scaleY / 2;
	float x = tile.posX;
	float y = tile.posY;
	float z = tile.posZ;
	float rot = tile.rotation;

	//verticies ------------------------------------
	makeQuad(vertexOffset, 
		x + getX(-w, -h, rot), y + getY(-w, -h, rot),z,
		x + getX(w, -h, rot),  y + getY(w, -h, rot),z,
		x + getX(-w, h, rot),  y + getY(-w, h, rot),z,
		x + getX(w, h, rot),   y + getY(w, h, rot),z);*/



	//colors ---------------------------------------
	makeColorQuad(colorOffset, tile.col.a, tile.col.r*255, tile.col.g*255, tile.col.b*255);
	//----------------------------------------------
	actualBuffer->numSprites++;

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

	for (auto it = buffers.begin(); it != buffers.end(); ++it){
		SpriteBuffer* buff = (*it).second;

		if (buff->numSprites > 0){
			glVertexPointer(3, GL_FLOAT, 0, &buff->verts[0]);
			glColorPointer(4, GL_UNSIGNED_BYTE, 0, &buff->colors[0]);
			glTexCoordPointer(2, GL_FLOAT, 0, &buff->coords[0]);

			buff->texture->bind();
			glDrawArrays(GL_TRIANGLES, 0, buff->numSprites * 6);
			buff->texture->unbind();
		}
	}

	if (safeMode)
	{
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

void ofxSpriteSheetRenderer::addTexCoords(flipDirection f, int posX, int posY, float w, float h)
{
	int coordOffset = getCoordOffset();

	w *= actualBuffer->textureCoeffX;
	h *= actualBuffer->textureCoeffY;

	switch (f) {
	case F_NONE:
		addTexCoords(coordOffset, posX, posY, posX+w, posY+h);
		break;
	case F_HORIZ:
		addTexCoords(coordOffset, posX+w, posY, posX, posY+h);
		break;
	case F_VERT:
		addTexCoords(coordOffset, posX, posY+h, posX+w, posY);
		break;
	case F_HORIZ_VERT:
		addTexCoords(coordOffset, posX+w, posY+h, posX, posY);
		break;
	default:
		break;
	}
}

void ofxSpriteSheetRenderer::addTexCoords(Tile& tile){

	int coordOffset = getCoordOffset();

	int x1, y1, x2, y2;

	switch (tile.dir) {
	case F_NONE:
		x1 = tile.offsetX;
		y1 = tile.offsetY;
		x2 = tile.offsetX + tile.width*actualBuffer->textureCoeffX;
		y2 = tile.offsetY + tile.height*actualBuffer->textureCoeffY;
		break;
	case F_HORIZ:
		x1 = tile.offsetX + tile.width*actualBuffer->textureCoeffX;
		y1 = tile.offsetY;
		x2 = tile.offsetX;
		y2 = tile.offsetY + tile.height*actualBuffer->textureCoeffY;
		break;
	case F_VERT:
		x1 = tile.offsetX;
		y1 = tile.offsetY + tile.height*actualBuffer->textureCoeffY;
		x2 = tile.offsetX + tile.width*actualBuffer->textureCoeffX;
		y2 = tile.offsetY;
		break;
	case F_HORIZ_VERT:
		x1 = tile.offsetX + tile.width*actualBuffer->textureCoeffX;
		y1 = tile.offsetY + tile.height*actualBuffer->textureCoeffY;
		x2 = tile.offsetX;
		y2 = tile.offsetY;
		break;
	default:
		break;
	}

	float* coords = actualBuffer->coords;

	coords[coordOffset] = coords[coordOffset + 4] = coords[coordOffset + 8] = x1;
	coords[coordOffset + 1] = coords[coordOffset + 3] = coords[coordOffset + 7] = y1;
	coords[coordOffset + 2] = coords[coordOffset + 6] = coords[coordOffset + 10] = x2;
	coords[coordOffset + 5] = coords[coordOffset + 9] = coords[coordOffset + 11] = y2;
}


bool ofxSpriteSheetRenderer::addRect(float x, float y, float z, float w, float h, float scale, float rot, int r, int g, int b, int alpha)
{

	if (actualBuffer == nullptr){
		cerr << "RENDER ERROR: Sheet doesn't exist!" << endl;
		return false;
	}

	if (actualBuffer->texture == nullptr)
	{
		cerr << "RENDER ERROR: No texture loaded!" << endl;
		return false;
	}

	if (actualBuffer->numSprites >= actualBuffer->bufferSize)
	{
		cerr << "RENDER ERROR: Sprites over allocated! Max " << actualBuffer->bufferSize << " sprites!" << endl;
		return false;
	}

	if (brushIndex < 0)
	{
		cerr << "RENDER ERROR: brush not initialized" << endl;
		return false;
	}

	int vertexOffset = getVertexOffset();
	int colorOffset = getColorOffset();
	int coordOffset = getCoordOffset();

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

	actualBuffer->numSprites++; // sprite == 2 triangles

	return true;
}


void ofxSpriteSheetRenderer::setTexCoeffs(int textureWidth, int textureHeight){
	actualBuffer->textureCoeffX = actualBuffer->textureCoeffY = 1;

#ifdef TARGET_OPENGLES	// if we don't have arb, it's crazy important that things are power of 2 so that this float is set properly
	actualBuffer->textureCoeffX = 1.0f / textureWidth;
	actualBuffer->textureCoeffY = 1.0f / textureHeight;
#endif
}

void ofxSpriteSheetRenderer::setBrushIndex(int index, int wh)
{
	brushIndex = index;
	brushSize = actualBuffer->textureCoeffX* wh;
	halfBrushSize = brushSize / 2;
}
