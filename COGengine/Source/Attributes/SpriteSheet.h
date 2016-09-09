#pragma once

#include "ofxCogMain.h"
#include "SpriteSet.h"


namespace Cog {

	/**
	* Entity for sprite sheet settings
	*/
	class SpriteSheet {
	private:
		vector<spt<SpriteSet>> sprites;
		spt<ofImage> spriteImage;
		string name;

	public:
		SpriteSheet(string name, spt<ofImage> spriteImage) : name(name), spriteImage(spriteImage) {
			sprites = vector<spt<SpriteSet>>();
		}

		spt<SpriteSet> AddSpriteSet(int offsetX, int offsetY, int totalFrames, int spriteWidth, int spriteHeight, int spriteSetWidth, int spriteSetHeight) {
			SpriteSet* sprite = new SpriteSet(this, offsetX, offsetY, totalFrames, spriteWidth, spriteHeight, spriteSetWidth, spriteSetHeight,
				this->spriteImage->getWidth(), this->spriteImage->getHeight());
			spt<SpriteSet> sptSprite = spt<SpriteSet>(sprite);

			sprites.push_back(sptSprite);

			return sptSprite;
		}

		bool RemoveSprite(spt<SpriteSet> sprite) {
			auto found = find(sprites.begin(), sprites.end(), sprite);

			if (found != sprites.end()) {
				sprites.erase(found);
				return true;
			}
			else return false;
		}

		string GetName() {
			return name;
		}

		spt<ofImage> GetSpriteImage() {
			return spriteImage;
		}

		vector<spt<SpriteSet>>& GetSprites() {
			return sprites;
		}
	};

}// namespace