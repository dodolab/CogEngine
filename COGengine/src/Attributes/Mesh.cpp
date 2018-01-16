#include "Mesh.h"
#include "Node.h"
#include "ofRectangle.h"

namespace Cog{

	void Mesh::UpdateBoundingBox(Trans& transform) {
		auto absPos = ofVec2f(transform.absPos.x, transform.absPos.y);
		this->boundingBox.topLeft = absPos;
		this->boundingBox.topRight = ofVec2f(absPos.x + this->GetWidth() * transform.absScale.x, absPos.y);
		this->boundingBox.bottomLeft = ofVec2f(absPos.x, absPos.y + this->GetHeight() * transform.absScale.y);
		this->boundingBox.bottomRight = ofVec2f(this->boundingBox.bottomLeft.x + this->GetWidth()* transform.absScale.x, this->boundingBox.bottomLeft.y);
	}


	void MultiSpriteMesh::Recalc() {

		int minX = 0;
		int minY = 0;
		int maxX = 0;
		int maxY = 0;
		int mWidth = 0;
		int mHeight = 0;

		for (int i = 0; i<sprites.size(); i++) {
			Sprite* spr = sprites[i];
			Trans& transform = spr->GetTransform();

			int posX = (int)transform.localPos.x;
			int posY = (int)transform.localPos.y;

			if (posX <= minX) minX = posX;
			if (posY <= minY) minY = posY;

			if (posX >= maxX) {
				maxX = posX;
				mWidth = spr->GetWidth();
			}

			if (posY >= maxY) {
				maxY = posY;
				mHeight = spr->GetHeight();
			}
		}

		// set size of bounding box of all sprite coordinates
		this->width = (maxX - minX) + mWidth;
		this->height = (maxY - minY) + mHeight;

		RefreshZIndex();
	}

	void MultiSpriteMesh::RefreshZIndex() {
		sort(sprites.begin(), sprites.end(),
			[](const Sprite*  a, const Sprite* b) -> bool {
			return a->GetZIndex() < b->GetZIndex();
		});
	}


	void BoundingBoxMesh::Recalc(Node* owner) {

		float minX = 1000000;
		float minY = 1000000;
		float maxX = -1000000;
		float maxY = -1000000;

		for (auto& child : owner->GetChildren()) {
			auto& trans = child->GetTransform();
			if (trans.absPos.x < minX) minX = trans.absPos.x;

			if (trans.absPos.y < minY) minY = trans.absPos.y;

			if ((trans.absPos.x + child->GetMesh()->GetWidth()*child->GetTransform().absScale.x) > maxX) 
				maxX = trans.absPos.x + child->GetMesh()->GetWidth()*child->GetTransform().absScale.x;
			if ((trans.absPos.y + child->GetMesh()->GetHeight()*child->GetTransform().absScale.y) > maxY) 
				maxY = trans.absPos.y + child->GetMesh()->GetHeight()*child->GetTransform().absScale.y;
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