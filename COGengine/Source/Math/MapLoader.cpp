#include "ofFbo.h"
#include "ofGraphics.h"
#include "MapLoader.h"
#include "Sprite.h"
#include "SpriteSheet.h"

namespace Cog {

	void MapLoader::LoadFromTextFile(string filename, Setting& idNameMap, TileMap& map) {

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


					Tile tile(hexa, index, name, i, j);
					map.tiles.push_back(tile);

					line.erase(0, pos + 1);

					j++;
					if (j > width) width = j;
				}

				i++;
				if (i > height) height = i;
			}
		}

		map.width = width;
		map.height = height;

	}

	void MapLoader::LoadFromPNGImage(string filename, Setting& idNameMap, TileMap& map) {
		ofFloatPixels pix;
		ofLoadImage(pix, filename);

		for (int i = 0; i < pix.getHeight(); i++) {
			for (int j = 0; j < pix.getWidth(); j++) {
				ofFloatColor color = pix.getColor(j, i);

				// in android, channels b and r are swapped
#if ANDROID
				int hexa = (((int)(color.b * 255) & 0xff) << 16) + ((((int)(color.g * 255)) & 0xff) << 8) + (((int)(color.r * 255)) & 0xff);
#else
				int hexa = (((int)(color.r * 255) & 0xff) << 16) + ((((int)(color.g * 255)) & 0xff) << 8) + (((int)(color.b * 255)) & 0xff);
#endif
				std::stringstream stream;
				stream << "0x" << std::setfill('0') << std::setw(6) << std::hex << hexa;

				std::string identifier(stream.str());

				auto values = idNameMap.GetItemVals(identifier);
				int index = 0;

				if (values.size() == 2) index = ofToInt(values[1]);
				string name = values[0];

				Tile tile(hexa, index, name, j, i);
				map.tiles.push_back(tile);
			}
		}

		map.width = pix.getWidth();
		map.height = pix.getHeight();
	}

	void MapLoader::GenerateMapImage(TileMap& tiles, map<string, vector<Sprite>>& sprites, 
		spt<SpriteSheet> spriteSheet, string filePath, float scale) {
		int mapWidth = tiles.width;
		int mapHeight = tiles.height;
		int spriteWidth = (*sprites.begin()).second[0].GetWidth();
		int spriteHeight = (*sprites.begin()).second[0].GetHeight();

		int totalWidth = mapWidth*spriteWidth;
		int totalHeight = mapHeight*spriteHeight;

		auto image = spriteSheet->GetSpriteImage();

		// render to fbo and then save it as an image
		ofFbo fbo = ofFbo();
		fbo.allocate(totalWidth*scale, totalHeight*scale, GL_RGBA);
		fbo.begin();
		ofClear(0, 0, 0, 0);
		ofSetColor(255);
		ofScale(scale, scale, scale);

		for (int i = 0; i < mapWidth; i++) {
			for (int j = 0; j < mapHeight; j++) {
				auto tile = tiles.GetTile(i, j);
				string name = tile.name;
				auto sprite = sprites[name][tile.index];
				image->drawSubsection(i*spriteWidth, j*spriteHeight, spriteWidth, spriteHeight, 
					sprite.GetPosX(), sprite.GetPosY(), spriteWidth, spriteHeight);
			}
		}

		fbo.end();
		fbo.draw(0, 0);
		ofPixels pixels = ofPixels();
		fbo.readToPixels(pixels);
		ofSaveImage(pixels, filePath);
	}

} // namespace