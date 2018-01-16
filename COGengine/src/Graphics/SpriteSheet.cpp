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

} // namespace