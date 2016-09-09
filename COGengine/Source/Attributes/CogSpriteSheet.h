#pragma once

#include "ofxCogMain.h"
#include "CogSpriteSet.h"


namespace Cog {

	/**
	* Entity for sprite sheet settings
	*/
	class CogSpriteSheet {
	private:
		vector<spt<CogSpriteSet>> sprites;
		spt<ofImage> spriteImage;
		string name;

	public:
		CogSpriteSheet(string name, spt<ofImage> spriteImage) : name(name), spriteImage(spriteImage) {
			sprites = vector<spt<CogSpriteSet>>();
		}

		spt<CogSpriteSet> AddSpriteSet(int offsetX, int offsetY, int totalFrames, int spriteWidth, int spriteHeight, int spriteSetWidth, int spriteSetHeight) {
			CogSpriteSet* sprite = new CogSpriteSet(this, offsetX, offsetY, totalFrames, spriteWidth, spriteHeight, spriteSetWidth, spriteSetHeight,
				this->spriteImage->getWidth(), this->spriteImage->getHeight());
			spt<CogSpriteSet> sptSprite = spt<CogSpriteSet>(sprite);

			sprites.push_back(sptSprite);

			return sptSprite;
		}

		bool RemoveSprite(spt<CogSpriteSet> sprite) {
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

		vector<spt<CogSpriteSet>>& GetSprites() {
			return sprites;
		}
	};

}