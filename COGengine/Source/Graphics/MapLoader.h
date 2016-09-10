#pragma once

#include "ofxCogCommon.h"
#include "Component.h"
#include "SheetAnim.h"
#include "Settings.h"
#include "CollageTexture.h"

namespace Cog {

	class Brick {
	public:
		int identifier;
		int index;
		string name;
		int posX;
		int posY;

		Brick(int identifier, int index, string name, int posX, int posY) : identifier(identifier), index(index), name(name), posX(posX), posY(posY) {

		}
	};

	class BrickMap {
	public:

		BrickMap() {

		}

		BrickMap(int width, int height, vector<Brick> bricks):width(width),height(height),bricks(bricks) {

		}

		vector<Brick> bricks;
		int width;
		int height;

		Brick& GetBrick(int x, int y) {
			return bricks[y*width + x];
		}
	};


	/**
	* Map loader -> transforms source file into set of bricks
	*/
	class MapLoader {

	public:

		/**
		* Loads list of bricks from text file
		* Text file must be in this format:
		*
		* x x x x
		* x x x x
		* x x x x
		* where x is brick identifier
		*
		* @param filename path to file
		* @param idNameMap map with identifier->human-readable-name, can be empty
		*/
		BrickMap LoadFromTextFile(string filename, Setting& idNameMap) {
			vector<Brick> bricks;

			ofBuffer buffer = ofBufferFromFile(filename);

			int width = 0;
			int height = 0;

			if (buffer.size()) {

				int i = 0;
				// parse lines
				for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
					string line = *it;
					size_t pos = 0;
					string identifier;

					int j = 0;

					// parse identifiers
					while ((pos = line.find(" ")) != string::npos) {
						identifier = line.substr(0, pos);
						
						int hexa = ofToInt(identifier);
						auto values = idNameMap.GetItemVals(identifier);
						int index = 0;

						if (values.size() == 2) index = ofToInt(values[1]);
						string name = values[0];


						Brick brick(hexa, index, name, i, j);
						bricks.push_back(brick);

						line.erase(0, pos + 1);

						j++;
						if (j > width) width = j;
					}

					i++;
					if (i > height) height = i;
				}
			}

			return BrickMap(width, height, bricks);
		}

		/**
		* Loads list of bricks from image
		* Each color will represent different type of brick
		* 
		* @param filename path to file
		* @param idNameMap map with identifier->human-readable-name, can be empty
		*/
		BrickMap* LoadFromPNGImage(string filename, Setting& idNameMap) {
			ofFloatPixels pix;
			ofLoadImage(pix, filename);

			vector<Brick> bricks;

			for (int i = 0; i < pix.getHeight(); i++) {
				for (int j = 0; j < pix.getWidth(); j++) {
					ofFloatColor color = pix.getColor(j, i);
					
					// bug in android: channels b and r are swapped
#if ANDROID
					int hexa = (((int)(color.b*255) & 0xff) << 16) + ((((int)(color.g*255)) & 0xff) << 8) + (((int)(color.r*255)) & 0xff);
#elif WIN32
					int hexa = (((int)(color.r * 255) & 0xff) << 16) + ((((int)(color.g * 255)) & 0xff) << 8) + (((int)(color.b * 255)) & 0xff);
#endif
					std::stringstream stream;
					stream << "0x" << std::setfill('0') << std::setw(6) << std::hex << hexa;

					std::string identifier(stream.str());

					auto values = idNameMap.GetItemVals(identifier);
					int index = 0;

					if (values.size() == 2) index = ofToInt(values[1]);
					string name = values[0];

					Brick brick(hexa, index, name, j, i);
					bricks.push_back(brick);
				}
			}

			return new BrickMap(pix.getWidth(), pix.getHeight(), bricks);
		}

		/**
		* Generates nice map image from sprites
		*/
		void GenerateMapImage(BrickMap* bricks, map<string, vector<Sprite>>& sprites, spt<SpriteSheet> spriteSheet, string filePath, float scale) {
			int mapWidth = bricks->width;
			int mapHeight = bricks->height;
			int spriteWidth = (*sprites.begin()).second[0].GetWidth();
			int spriteHeight = (*sprites.begin()).second[0].GetHeight();

			int totalWidth = mapWidth*spriteWidth;
			int totalHeight = mapHeight*spriteHeight;
			
			auto image = spriteSheet->GetSpriteImage();

			ofFbo fbo = ofFbo();
			fbo.allocate(totalWidth*scale, totalHeight*scale, GL_RGBA);
			fbo.begin();
			ofClear(0,0,0,0);
			ofSetColor(255);
			ofScale(scale, scale, scale);

			for (int i = 0; i < mapWidth; i++) {
				for (int j = 0; j < mapHeight; j++) {
					auto brick = bricks->GetBrick(i, j);
					string name = brick.name;
					auto sprite = sprites[name][brick.index];
					image->drawSubsection(i*spriteWidth, j*spriteHeight, spriteWidth, spriteHeight,sprite.GetPosX(),sprite.GetPosY(),spriteWidth,spriteHeight);
				}
			}

			fbo.end();
			fbo.draw(0, 0);
			ofPixels pixels = ofPixels();
			fbo.readToPixels(pixels);
			ofSaveImage(pixels, filePath);

 		}

	};

} // namespace