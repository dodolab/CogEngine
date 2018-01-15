#include "SpriteSet.h"
#include "SpriteSheet.h"

namespace Cog {

	SpriteSet::SpriteSet(SpriteSheet* spriteSheet, string name, int totalFrames,
		int spriteWidth, int spriteHeight)
		: SpriteSet(spriteSheet, name, 0, 0, totalFrames, spriteWidth, spriteHeight, 0, 0) {

		spriteSetWidth = spriteSheet->GetSpriteImage()->getWidth();
		spriteSetHeight = spriteSheet->GetSpriteImage()->getHeight();
	}

	SpriteSet::SpriteSet(SpriteSheet* spriteSheet, string name, int totalFrames,
		int spriteWidth, int spriteHeight, int spriteSetWidth, int spriteSetHeight)
		: SpriteSet(spriteSheet, name, 0, 0, totalFrames, spriteWidth, spriteHeight, spriteSetWidth, spriteSetHeight) {

	}

	SpriteSet::SpriteSet(SpriteSheet* spriteSheet, string name, int offsetX, int offsetY, int totalFrames,
		int spriteWidth, int spriteHeight, int spriteSetWidth, int spriteSetHeight)
		: spriteSheet(spriteSheet), name(name), offsetX(offsetX), offsetY(offsetY), totalFrames(totalFrames), spriteWidth(spriteWidth),
		spriteHeight(spriteHeight), spriteSetWidth(spriteSetWidth), spriteSetHeight(spriteSetHeight) {

		spriteSheetWidth = spriteSheet->GetSpriteImage()->getWidth();
		spriteSheetHeight = spriteSheet->GetSpriteImage()->getHeight();
	}

} // namespace