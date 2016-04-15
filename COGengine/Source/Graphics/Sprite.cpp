#include "Sprite.h"

namespace Cog {


	Sprite::Sprite(spt<SpriteSet> spriteSet, int row, int column) {

		int columns = spriteSet->GetSpriteSetWidth() / spriteSet->GetSpriteWidth();
		this->frame = row*columns + column;
		Recalc(spriteSet);
	}

	Sprite::Sprite(spt<SpriteSet> spriteSet, int frame) : frame(frame) {
		Recalc(spriteSet);
	}

	void Sprite::Recalc(spt<SpriteSet> spriteSet) {
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
			// sprite forms a rectangle inside the sprite image
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
		int row = rows == 1 ? 0 : frameInGrid / (rows);

		posX = spriteSet->GetOffsetX() + column*width;
		posY = spriteSet->GetOffsetY() + row*height;
	}


} // namespace