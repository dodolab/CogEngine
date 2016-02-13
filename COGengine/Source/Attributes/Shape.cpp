#include "Shape.h"

namespace Cog{

	void SpritesShape::Recalc() {

		int minX = 0;
		int minY = 0;
		int maxX = 0;
		int maxY = 0;
		int mWidth = 0;
		int mHeight = 0;

		for (auto it = sprites.begin(); it != sprites.end(); ++it) {
			spt<Sprite> crt = (*it).first;
			Trans transform = (*it).second;

			int posX = (int)transform.localPos.x;
			int posY = (int)transform.localPos.y;

			if (posX < minX) minX = posX;
			if (posY < minY) minY = posY;

			if (posX > maxX) {
				maxX = posX;
				mWidth = crt->GetWidth();
			}

			if (posY > maxY) {
				maxY = posY;
				mHeight = crt->GetHeight();
			}
		}

		this->width = (maxX - minX) + mWidth;
		this->height = (maxY - minY) + mHeight;
	}

}// namespace