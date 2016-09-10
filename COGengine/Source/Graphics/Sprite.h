#pragma once

#include "Definitions.h"
#include "SpriteSet.h"

namespace Cog {

	/**
	* Sprite entity, a part of a texture image called sprite sheet
	* Identified by frame index, pointing to the area on the sheet
	*/
	class Sprite {
	private:
		int frame;

		int posX;
		int posY;
		int width;
		int height;

	public:

		Sprite() {

		}

		/**
		* Creates a new sprite
		* @param spriteSet collection of sprites
		* @param row index of a row in which this sprite is located
		* @param column index of a column in which this sprite is located
		*/
		Sprite(spt<SpriteSet> spriteSet, int row, int column);

		/**
		* Creates a new sprite
		* @param spriteSet collection of sprites
		* @param frame index of a frame on the sprite sheet
		*/
		Sprite(spt<SpriteSet> spriteSet, int frame);

		int GetFrame() const {
			return frame;
		}

		int GetPosX() const {
			return posX;
		}

		int GetPosY() const {
			return posY;
		}

		int GetWidth() const {
			return width;
		}

		int GetHeight() const {
			return height;
		}

	protected:
		/**
		* Recalculates attributes (width, height), according to values in sprite set
		*/
		void Recalc(spt<SpriteSet> spriteSet);
	};

}// namespace