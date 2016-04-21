#include "SpriteSheet.h"
#include "Error.h"
#include "Facade.h"

namespace Cog {

	SpriteSheet::SpriteSheet(string name, spt<ofImage> spriteImage) : name(name), spriteImage(spriteImage) {
		sprites = vector<spt<SpriteSet>>();
	}

	SpriteSheet::SpriteSheet(string name, spt<ofImage> spriteImage, int totalFrames, int spriteWidth, int spriteHeight) : SpriteSheet(name, spriteImage) {
		AddSpriteSet(name, totalFrames, spriteWidth, spriteHeight);
	}

	spt<SpriteSet> SpriteSheet::AddSpriteSet(string name, int totalFrames, int spriteWidth, int spriteHeight) {
		return AddSpriteSet(name, 0, 0, totalFrames, spriteWidth, spriteHeight, (int)spriteImage->getWidth(), (int)spriteImage->getHeight());
	}

	spt<SpriteSet> SpriteSheet::AddSpriteSet(string name, int totalFrames, int spriteWidth, int spriteHeight, int spriteSetWidth, int spriteSetHeight) {
		return AddSpriteSet(name, 0, 0, totalFrames, spriteWidth, spriteHeight, spriteSetWidth, spriteSetHeight);
	}

	spt<SpriteSet> SpriteSheet::AddSpriteSet(string name, int offsetX, int offsetY, int totalFrames, int spriteWidth, int spriteHeight, int spriteSetWidth, int spriteSetHeight) {
		SpriteSet* sprite = new SpriteSet(this, name, offsetX, offsetY, totalFrames, spriteWidth, spriteHeight, spriteSetWidth, spriteSetHeight);
		spt<SpriteSet> sptSprite = spt<SpriteSet>(sprite);

		sprites.push_back(sptSprite);

		return sptSprite;
	}

	bool SpriteSheet::RemoveSprite(spt<SpriteSet> sprite) {
		auto found = find(sprites.begin(), sprites.end(), sprite);

		if (found != sprites.end()) {
			sprites.erase(found);
			return true;
		}
		else return false;
	}

	void SpriteSheet::LoadFromXml(spt<ofxXml> xml) {
		int spriteSets = xml->getNumTags("spriteset");

		string name = xml->getAttributex("name", "");
		if (name.empty()) throw new IllegalArgumentException("SpriteSheet must have name specified!");
		string img = xml->getAttributex("img", "");
		if (img.empty()) throw new IllegalArgumentException("SpriteSheet must have img specified!");

		this->name = name;
		// todo: maybe lazy load ?
		this->spriteImage = CogGet2DImage(img);

		if (spriteSets == 0) {
			// 0 spritesets -> attributes are directly in spritesheet node
			int frames = xml->getAttributex("frames", 0);
			int spriteWidth = xml->getAttributex("sprite_width", 0);
			int spriteHeight = xml->getAttributex("sprite_height", 0);
			AddSpriteSet("default", frames, spriteWidth, spriteHeight);
		}
		else {
			for (int i = 0; i < spriteSets; i++) {
				xml->pushTag("spriteset", i);

				string name = xml->getAttributex("name", "");
				int frames = xml->getAttributex("frames", 0);
				int spriteWidth = xml->getAttributex("sprite_width", 0);
				int spriteHeight = xml->getAttributex("sprite_height", 0);

				AddSpriteSet(name, frames, spriteWidth, spriteHeight);

				xml->popTag();
			}
		}
	}

} // namespace