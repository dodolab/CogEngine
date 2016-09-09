#pragma once

#include "ofxCogMain.h"
#include "CogSpriteSheet.h"

namespace Cog {

	class CogSprite {
	private:
		spt<CogSpriteSet> spriteSet;
		int frame;

		int posX;
		int posY;
		int width;
		int height;

		void Recalc() {
			width = spriteSet->GetSpriteWidth();
			height = spriteSet->GetSpriteHeight();

			int frameInGrid = 0;
			int totalFramesInGrid = 0;

			if (spriteSet->GetSpriteSetWidth() == spriteSet->GetSpriteSheetWidth()) {
				// sprite set is continuous; it means that it begins somewhere and when it reaches the end 
				// of the image, the next sprite will be at the beginning of the image
				// EXAMPLE (x for sprite):
				// * * * x x
				// x x x x x
				// x x * * *
				// * * * * *
				// * * * * *
				int firstRowColumns = (spriteSet->GetSpriteSheetWidth() - spriteSet->GetOffsetX()) / width;
				int otherRowsColumns = spriteSet->GetSpriteSheetWidth() / width;

				frameInGrid = frame + (otherRowsColumns - firstRowColumns);
				totalFramesInGrid = (otherRowsColumns - firstRowColumns) + spriteSet->GetTotalFrames();
			}
			else {
				// if sprite set isn't continuous, it means that it forms rectangle inside the sprite image
				// and the first frame is at the very beginning of this rectangle
				// EXAMPLE (x for sprite):
				// * * x x *
				// * * x x *
				// * * * * *
				// * * * * *
				// * * * * *

				frameInGrid = frame;
			}

			int columns = spriteSet->GetSpriteSetWidth() / width;
			int rows = (totalFramesInGrid*width) / spriteSet->GetSpriteSetWidth();

			int column = frameInGrid%columns;
			int row = frameInGrid / rows;

			posX = spriteSet->GetOffsetX() + column*width;
			posY = spriteSet->GetOffsetY() + row*height;

		}

	public:

		CogSprite(spt<CogSpriteSet> spriteSet, int frame) : spriteSet(spriteSet), frame(frame) {
			Recalc();
		}

		spt<CogSpriteSet> GetSpriteSet() {
			return spriteSet;
		}

		int GetFrame() {
			return frame;
		}

		int GetPosX() {
			return posX;
		}

		int GetPosY() {
			return posY;
		}

		int GetWidth() {
			return width;
		}

		int GetHeight() {
			return height;
		};

	};

}