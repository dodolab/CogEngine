#pragma once

#include "ofxCogCommon.h"
#include "Component.h"
#include "SheetAnim.h"
#include "Settings.h"

namespace Cog {

	class Brick {
	public:
		int identifier;
		string name;
		int posX;
		int posY;

		Brick(int identifier, string name, int posX, int posY) : identifier(identifier), name(name), posX(posX), posY(posY) {

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
			vector<Brick> bricks = vector<Brick>();

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
						string name = idNameMap.GetItemVal(identifier);


						Brick brick(hexa, name, i, j);
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

			vector<Brick> bricks = vector<Brick>();

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

					string name = idNameMap.GetItemVal(identifier);
					
					Brick brick(hexa, name, j, i);
					bricks.push_back(brick);
				}
			}

			return new BrickMap(pix.getWidth(), pix.getHeight(), bricks);
		}


	};

} // namespace