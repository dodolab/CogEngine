#pragma once

#include <string>
#include "Definitions.h"

using namespace std;

namespace Cog {

	class SpriteSheet;

	/**
	* Collection of sprites that is a part of a sprite sheet
	* Each sprite sheet may contain more sprite sets with different names, on different offset and
	* with various frames.
	*
	* It is suitable for animations where each animated object has its all frames in one sprite set
	*/
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
		
		/**
		* Creates a new sprite set
		* @param spriteSheet sprite sheet in which this set comprises
		* @param name name of the set
		* @param totalFrames number of frames in the set
		* @param spriteWidth width of one sprite
		* @param spriteHeight height of one sprite
		*/
		SpriteSet(SpriteSheet* spriteSheet, string name, int totalFrames,
			int spriteWidth, int spriteHeight);

		/**
		* Creates a new sprite set
		* @param spriteSheet sprite sheet in which this set comprises
		* @param name name of the set
		* @param totalFrames number of frames in the set
		* @param spriteWidth width of one sprite
		* @param spriteHeight height of one sprite
		* @param spriteSetWidth width of the whole sprite set
		* @param spriteSetHeight height of the whole sprite set
		*/
		SpriteSet(SpriteSheet* spriteSheet, string name, int totalFrames,
			int spriteWidth, int spriteHeight, int spriteSetWidth, int spriteSetHeight);

		/**
		* Creates a new sprite set
		* @param spriteSheet sprite sheet in which this set comprises
		* @param name name of the set
		* @param offsetX position in the sheet on the x axis where the sprite set starts
		* @param offsetY position in the sheet on the y axis where the sprite set starts
		* @param totalFrames number of frames in the set
		* @param spriteWidth width of one sprite
		* @param spriteHeight height of one sprite
		* @param spriteSetWidth width of the whole sprite set
		* @param spriteSetHeight height of the whole sprite set
		*/
		SpriteSet(SpriteSheet* spriteSheet, string name, int offsetX, int offsetY, int totalFrames,
			int spriteWidth, int spriteHeight, int spriteSetWidth, int spriteSetHeight);

		SpriteSheet* GetSpriteSheet() {
			return spriteSheet;
		}

		string GetName() const {
			return name;
		}

		int GetOffsetX() const {
			return offsetX;
		}

		int GetOffsetY() const {
			return offsetY;
		}

		int GetTotalFrames() const {
			return totalFrames;
		}

		int GetSpriteWidth() const {
			return spriteWidth;
		}

		int GetSpriteHeight() const {
			return spriteHeight;
		}

		int GetSpriteSetWidth() const {
			return spriteSetWidth;
		}

		int GetSpriteSetHeight() const {
			return spriteSetHeight;
		}

		int GetSpriteSheetWidth() const {
			return spriteSheetWidth;
		}

		int GetSpriteSheetHeight() const {
			return spriteSheetHeight;
		}
	};

}// namespace