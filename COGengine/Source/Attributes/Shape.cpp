#include "Shape.h"
#include "Node.h"

namespace Cog{

	void SpritesShape::Recalc() {

		int minX = 0;
		int minY = 0;
		int maxX = 0;
		int maxY = 0;
		int mWidth = 0;
		int mHeight = 0;

		for (auto it = sprites.begin(); it != sprites.end(); ++it) {
			spt<Sprite> crt = (*it)->sprite;
			Trans transform = (*it)->transform;

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

		RefreshZIndex();

	}

	void BoundingBox::Recalc(Node* owner) {

		float minX = 10000;
		float minY = 10000;
		float maxX = -10000;
		float maxY = -10000;

		for (auto& child : owner->GetChildren()) {
			auto& trans = child->GetTransform();
			if (trans.absPos.x < minX) minX = trans.absPos.x;

			if (trans.absPos.y < minY) minY = trans.absPos.y;

			if ((trans.absPos.x + child->GetShape()->GetWidth()*child->GetTransform().absScale.x) > maxX) 
				maxX = trans.absPos.x + child->GetShape()->GetWidth()*child->GetTransform().absScale.x;
			if ((trans.absPos.y + child->GetShape()->GetHeight()*child->GetTransform().absScale.y) > maxY) 
				maxY = trans.absPos.y + child->GetShape()->GetHeight()*child->GetTransform().absScale.y;
		}

		if (margin != 0) {
			minX -= (maxX - minX)*margin / 2;
			maxX += (maxX - minX)*margin / 2;
			minY -= (maxY - minY)*margin / 2;
			maxY += (maxY - minY)*margin / 2;
		}

		boundingBox = ofRectangle(minX, minY, (maxX - minX), (maxY - minY));
	}

}// namespace