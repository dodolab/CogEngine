#ifndef OFX_SPRITE_SHEET_RENDERER_H
#define OFX_SPRITE_SHEET_RENDERER_H

#include "ofMain.h"
#include "CollageTexture.h"
#include "PixelTexture.h"
#include "LinearTexture.h"


enum flipDirection { F_NONE = 0, F_HORIZ, F_VERT, F_HORIZ_VERT };

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
	void clearCounters(int layer = -1);

	bool addTile(int tile_name, int frame, float x, float y, float z, int layer = -1, float w = 1, float h = 1, flipDirection f = F_NONE, float scale = 1.0, float rot = 0, int r = 255, int g = 255, int b = 255, int alpha = 255);

	void draw();

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

	ofTexture* texture;

	int numLayers;
	int defaultLayer;
	int tilesPerLayer;
	int tileSize;

	float* verts;
	float* coords;
	unsigned char * colors;

	int* numSprites;
	int spriteSheetWidth;

	void getFrameXandY(int tile_position, float &x, float &y);

	ofPoint getFramePosOnSheet(float _frameX, float _frameY, int _frame, float _w); // Added by J. Matthew Griffis to support multi-row animation. Returns the x and y of the frame as a point.

	float getX(int x, int y, float angle);
	float getY(int x, int y, float angle);

	void addTexCoords(flipDirection f, float &frameX, float &frameY, int layer, float x = 1, float y = 1);

	// creates quad for vertex array
	void makeQuad(int offset, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4);
	
	// creates quad for color array
	void makeColorQuad(int offset, int a, int r, int g, int b);
	
	// adds texture coordinates to texture array
	void addTexCoords(int offset, float x1, float y1, float x2, float y2);

	int getLayerOffset(int layer){
		return layer*tilesPerLayer;
	}

	int getVertexOffset(int layer){
		return  (getLayerOffset(layer) + numSprites[layer]) * 18;
	}

	int getColorOffset(int layer){
		return (getLayerOffset(layer) + numSprites[layer]) * 24;
	}

	int getCoordOffset(int layer){
		return (getLayerOffset(layer) + numSprites[layer]) * 12;
	}
};

#endif