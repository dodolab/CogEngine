#include "HitBox.h"
#include "Node.h"

namespace Cog {

	void HitBox::Recalc(Node* owner) {

		float minX = 10000;
		float minY = 10000;
		float maxX = -10000;
		float maxY = -10000;

		for (auto& child : owner->GetChildren()) {
			auto& trans = child->GetTransform();
			if (trans.absPos.x < minX) minX = trans.absPos.x;

			if (trans.absPos.y < minY) minY = trans.absPos.y;

			if ((trans.absPos.x + owner->GetShape()->GetWidth()) > maxX) maxX = trans.absPos.x + owner->GetShape()->GetWidth();
			if ((trans.absPos.y + owner->GetShape()->GetHeight()) > maxY) maxY = trans.absPos.y + owner->GetShape()->GetHeight();
		}

		this->posX = minX;
		this->posY = minY;
		this->width = (maxX - minX);
		this->height = (maxY - minY);
	}


} // namespace