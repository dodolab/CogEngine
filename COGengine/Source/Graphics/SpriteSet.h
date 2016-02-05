#pragma once

#include "ofxCogCommon.h"

namespace Cog {

	class SpriteSheet;

	class SpriteSet {
	private:
		int offsetX;
		int offsetY;
		int totalFrames;
		int spriteWidth;
		int spriteHeight;
		int spriteSetWidth;
		int spriteSetHeight;

		SpriteSheet* spriteSheet;

		int spriteSheetWidth;
		int spriteSheetHeight;
		string name;

	public:
		SpriteSet(SpriteSheet* spriteSheet, string name, int totalFrames,
			int spriteWidth, int spriteHeight);

		SpriteSet(SpriteSheet* spriteSheet, string name, int totalFrames,
			int spriteWidth, int spriteHeight, int spriteSetWidth, int spriteSetHeight);

		SpriteSet(SpriteSheet* spriteSheet, string name, int offsetX, int offsetY, int totalFrames,
			int spriteWidth, int spriteHeight, int spriteSetWidth, int spriteSetHeight);

		string GetName() {
			return name;
		}

		int GetOffsetX() {
			return offsetX;
		}

		int GetOffsetY() {
			return offsetY;
		}

		int GetTotalFrames() {
			return totalFrames;
		}

		int GetSpriteWidth() {
			return spriteWidth;
		}

		int GetSpriteHeight() {
			return spriteHeight;
		}

		int GetSpriteSetWidth() {
			return spriteSetWidth;
		}

		int GetSpriteSetHeight() {
			return spriteSetHeight;
		}

		int GetSpriteSheetWidth() {
			return spriteSheetWidth;
		}

		int GetSpriteSheetHeight() {
			return spriteSheetHeight;
		}
	};

}// namespace