#include "SpriteSheet.h"
#include "Facade.h"

namespace Cog {

	SpriteSheet::SpriteSheet(spt<ofImage> spriteAtlas, string spriteAtlasName, string sheetName, int totalFrames,
		int spriteWidth, int spriteHeight)
		: SpriteSheet(spriteAtlas, spriteAtlasName, sheetName, 0, 0, totalFrames, spriteWidth, spriteHeight, 0, 0) {

		spriteSheetWidth = spriteAtlas->getWidth();
		spriteSheetHeight = spriteAtlas->getHeight();
	}

	SpriteSheet::SpriteSheet(spt<ofImage> spriteAtlas, string spriteAtlasName, string sheetName, int totalFrames,
		int spriteWidth, int spriteHeight, int spriteSheetWidth, int spriteSheetHeight)
		: SpriteSheet(spriteAtlas, spriteAtlasName, sheetName, 0, 0, totalFrames, spriteWidth, spriteHeight, spriteSheetWidth, spriteSheetHeight) {

	}

	SpriteSheet::SpriteSheet(spt<ofImage> spriteAtlas, string spriteAtlasName, string sheetName, int offsetX, int offsetY, int totalFrames,
		int spriteWidth, int spriteHeight, int spriteSheetWidth, int spriteSheetHeight)
		: spriteAtlas(spriteAtlas), spriteAtlasName(spriteAtlasName), name(sheetName), offsetX(offsetX), offsetY(offsetY), totalFrames(totalFrames), spriteWidth(spriteWidth),
		spriteHeight(spriteHeight), spriteSheetWidth(spriteSheetWidth), spriteSheetHeight(spriteSheetHeight) {

		spriteAtlasWidth = spriteAtlas->getWidth();
		spriteAtlasHeight = spriteAtlas->getHeight();
	}

	void SpriteSheet::LoadFromXml(xml_node& xml, string img, string atlasName) {
		auto spriteSets = xml.children("spriteset");

		string name = xml.attribute("name").as_string(atlasName.c_str());
		
		this->name = name;
		this->spriteAtlas= CogGet2DImage(img);
		this->spriteAtlasName = atlasName;

		this->totalFrames = xml.attribute("frames").as_int(0);
		this->spriteWidth = xml.attribute("sprite_width").as_int(0);
		this->spriteHeight = xml.attribute("sprite_height").as_int(0);
	}

} // namespace