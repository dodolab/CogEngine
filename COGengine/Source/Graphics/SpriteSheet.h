#pragma once

#include <string>
#include <vector>
using namespace std;
#include "ofImage.h"
#include "ofxXmlSettings.h"

#include "Definitions.h"
#include "SpriteSet.h"


namespace Cog {

	/**
	* Entity for sprite sheet, a texture with frames, each of which can be rendered separately
	*/
	class SpriteSheet {
	private:
		// collection of sprites
		vector<spt<SpriteSet>> sprites;
		// texture image
		spt<ofImage> spriteImage;
		// name of the sheet
		string name;

	public:

		SpriteSheet() {

		}

		SpriteSheet(string name, spt<ofImage> spriteImage);

		SpriteSheet(string name, spt<ofImage> spriteImage, int totalFrames, int spriteWidth, int spriteHeight);

		/**
		* Inserts a new sprite into sheet
		* @param totalFrames number of frames
		* @param spriteWidth width of one frame
		* @param spriteHeight height of one frame
		*/
		spt<SpriteSet> AddSpriteSet(string name, int totalFrames, int spriteWidth, int spriteHeight);

		/**
		* Inserts a new sprite into sheet
		* @param totalFrames number of frames
		* @param spriteWidth width of one frame
		* @param spriteHeight height of one frame
		* @param spriteSetWidth width of the whole sheet (usually width of the whole image)
		* @param spriteSetHeight height of the whole sheet (usually height of the whole image)
		*/
		spt<SpriteSet> AddSpriteSet(string name, int totalFrames, int spriteWidth, int spriteHeight, int spriteSetWidth, int spriteSetHeight);

		/**
		* Inserts a new sprite into sheet
		* @param offsetX offset of the first frame in the picture on x-axis (0 in default)
		* @param offsetY offset of the first frame in the picture on y-axis (0 in default)
		* @param totalFrames number of frames
		* @param spriteWidth width of one frame
		* @param spriteHeight height of one frame
		* @param spriteSetWidth width of the whole sheet (usually width of the whole image)
		* @param spriteSetHeight height of the whole sheet (usually height of the whole image)
		*/
		spt<SpriteSet> AddSpriteSet(string name, int offsetX, int offsetY, int totalFrames, int spriteWidth, int spriteHeight, int spriteSetWidth, int spriteSetHeight);

		string GetName() const {
			return name;
		}

		spt<ofImage> GetSpriteImage() {
			return spriteImage;
		}

		/**
		* Gets the first sprite set
		*/
		spt<SpriteSet> GetDefaultSpriteSet() {
			return sprites[0];
		}

		/**
		* Finds sprite set by name
		*/
		spt<SpriteSet> GetSpriteSetByName(string name) {
			for (spt<SpriteSet> sprSet : sprites) {
				if (sprSet->GetName().compare(name) == 0) return sprSet;
			}

			return spt<SpriteSet>();
		}

		vector<spt<SpriteSet>>& GetSprites() {
			return sprites;
		}

		/**
		* Removes sprite from the collection
		*/
		bool RemoveSprite(spt<SpriteSet> sprite);


		void LoadFromXml(spt<ofxXml> xml);
	};

}// namespace