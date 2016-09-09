#pragma once

#include "ofMain.h"
#include "CollageTexture.h"
#include "SpriteTexture.h"

namespace Cog {

	enum flipDirection { F_NONE = 0, F_HORIZ, F_VERT, F_HORIZ_VERT };

	struct Tile {

		Tile() {
			offsetX = 0;
			offsetY = 0;
			width = 0;
			height = 0;
			posX = 0;
			posY = 0;
			posZ = 0;
			dir = flipDirection::F_NONE;
			scaleX = 1;
			scaleY = 1;
			rotation = 0;
			col = ofColor(1.0f);
		}

		int offsetX;
		int offsetY;
		int width;
		int height;
		float posX;
		float posY;
		float posZ;
		flipDirection dir;
		float scaleX;
		float scaleY;
		float rotation;
		ofColor col;
	};

	struct SpriteBuffer {

		SpriteBuffer() {
			texture = nullptr;
			verts = nullptr;
			coords = nullptr;
			colors = nullptr;
			bufferSize = 0;

			textureIsExternal = false;
		}

		~SpriteBuffer() {
			if (texture != nullptr) {
				if (textureIsExternal) texture->clear();
				else delete texture;
			}

			if (verts != nullptr)
				delete[] verts;
			if (coords != nullptr)
				delete[] coords;
			if (colors != nullptr)
				delete[] colors;
		}

		string name;
		float textureCoeffX;
		float textureCoeffY;
		bool textureIsExternal;

		ofTexture* texture;
		int bufferSize;

		float* verts;
		float* coords;
		unsigned char * colors;

		int numSprites;

	};


	//class -------------------------------------------
	class SpriteSheetRenderer
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
		SpriteSheetRenderer();
		~SpriteSheetRenderer();

		void reAllocateArrays(string sheetName, int bufferSize);

		/**
		* Loads texture from file
		* @param widthHeight - texture size (should be power of 2)
		* @param internalGLScaleMode - opengl scaling, possibles values: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST,
		* GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR
		*/
		void loadTexture(string fileName, string sheetName, int bufferSize, int width, int height, int internalGLScaleMode);
		void loadTexture(ofTexture * texture, string sheetNAme, int bufferSize, bool isExternal = true);
		void loadTexture(CollageTexture * texture, string sheetName, int bufferSize);
		void loadTexture(SpriteTexture * texture, string sheetName, int bufferSize);

		// -----------------------------------------
		void setBrushIndex(int index, int wh = 1);

		//shapes (require a brushTexture to be on the sprite sheet)
		bool addRect(float x, float y, float z, float w, float h, float scale, float rot, int r = 255, int g = 255, int b = 255, int alpha = 255);

		// -----------------------------------------
		void clearCounters(string sheetName);


		bool addTile(Tile& tile);

		void draw();

		// texture creation ------------------------
		void allocate(string sheetName, int textureWidth, int textureHeight, int internalGLScaleMode);

		void clearTexture(string sheetName);

		void addMisc(string fileName, string sheetName, int x, int y, int glType = GL_RGBA);

		void finishTexture(string sheetName);

		void setActualBuffer(string sheetName) {
			auto buff = buffers.find(sheetName);

			if (buff != buffers.end()) actualBuffer = buff->second;
			else actualBuffer = nullptr;
		}

		// -----------------------------------------

	protected:

		map<string, SpriteBuffer*> buffers;
		SpriteBuffer* actualBuffer;



		inline float getX(int x, int y, float angle) {
			return (angle == 0) ? x : ((float)x*cosf(angle) - (float)y*sinf(angle));
		}

		inline float getY(int x, int y, float angle) {
			return (angle == 0) ? y : ((float)x*sinf(angle) + (float)y*cosf(angle));
		}

		void setTexCoeffs(int textureWidth, int textureHeight);

		void addTexCoords(flipDirection f, int frameX, int frameY, float x = 1, float y = 1);

		void addTexCoords(Tile& tile);

		// creates quad for vertex array
		void makeQuad(int offset, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4);

		void makeQuad(int offset, Tile& tile);

		// creates quad for color array
		void makeColorQuad(int offset, int a, int r, int g, int b);

		// adds texture coordinates to texture array
		void addTexCoords(int offset, float x1, float y1, float x2, float y2);

		inline int getVertexOffset() {
			return  (actualBuffer->numSprites) * 18;
		}

		inline int getColorOffset() {
			return (actualBuffer->numSprites) * 24;
		}

		inline int getCoordOffset() {
			return (actualBuffer->numSprites) * 12;
		}
	};

}// namespace