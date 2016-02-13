#include "TransformMath.h"
#include "Node.h"
#include "Settings.h"

namespace Cog {

	void TransformMath::SetSizeToScreen(Node* node, Node* parent) {
		SetTransform(node, parent, TransformEnt("",ofVec2f(0, 0), 1, CalcType::PER, ofVec2f(0, 0), ofVec2f(1, 0) // zero because we want to scale according to the x axis
			, CalcType::ABS_PER,0), 0, 0);
	}

	void TransformMath::SetTransform(Node* node, Node* parent, TransformEnt entity, int gridWidth, int gridHeight) {

		Trans nodeTransform = Trans(0, 0);

		CalcTransform(nodeTransform, node, parent, entity, gridWidth, gridHeight);

		// refresh transform (recalculate from parent)
		nodeTransform.CalcAbsTransform(parent->GetTransform());

		node->SetTransform(nodeTransform);
	}

	void TransformMath::CalcTransform(Trans& outputTrans, Node* node, Node* parent, TransformEnt entity, int gridWidth, int gridHeight) {

		Trans& parentTrans = parent->GetTransform();

		// calculate scale
		ofVec2f scale = CalcScale(node, parent, entity.size.x, entity.size.y, entity.sType, gridWidth, gridHeight);
		// calculate position
		ofVec2f absPos = CalcPosition(node, parent, entity.pos, entity.pType, gridWidth, gridHeight);

		// fix position according to the anchor
		auto shape = node->GetShape();

		absPos.x += (0.0f - entity.anchor.x) * shape->GetWidth()*scale.x;
		absPos.y += (0.0f - entity.anchor.y) * shape->GetHeight()*scale.y;

		int zIndex = entity.zIndex;
		// if zIndex is equal to 0, the value will be taken from the parent
		if (zIndex == 0) zIndex = (int)parentTrans.localPos.z;

		// set transformation
		outputTrans.localPos = ofVec3f(absPos.x, absPos.y, (float)zIndex);
		outputTrans.scale = ofVec3f(scale.x, scale.y, 1);
		outputTrans.rotationOrigin = ofVec2f(shape->GetWidth(), shape->GetHeight()) * entity.rotationOrigin * (scale); // multiply by abs scale
		outputTrans.rotation = entity.rotation;
	}

	ofVec2f TransformMath::CalcPosition(Node* node, Node* parent, ofVec2f pos, CalcType posCalc, int gridWidth, int gridHeight) {
		Vec2i scrSize = CogGetVirtualScreenSize();
		Trans& parentTrans = parent->GetTransform();
		ofVec2f absPos;


		switch (posCalc) {
		case CalcType::ABS:
			// absolute position in device pixels
			absPos = ofVec2f((pos.x - parentTrans.absPos.x) / parentTrans.absScale.x,
				(pos.y - parentTrans.absPos.y) / parentTrans.absScale.y);
			break;
		case CalcType::LOC:
			// local position, is scaled according to the parent absolute scale
			absPos = pos;
			break;
		case CalcType::ABS_PER:
			// absolute percentage -> screen size is 1.0 x 1.0
			absPos = ofVec2f((pos.x*scrSize.x - parentTrans.absPos.x) / parentTrans.absScale.x,
				(pos.y*scrSize.y - parentTrans.absPos.y) / parentTrans.absScale.y);
			break;
		case CalcType::PER:
			// relative percentage -> parent size is 1.0 x 1.0
			absPos = ofVec2f(pos.x*parent->GetShape()->GetWidth(),
				pos.y*parent->GetShape()->GetHeight());
			break;
		case CalcType::GRID:
			// grid percentage -> grid size must be specified
			float percentagePosx = pos.x / gridWidth;
			float percentagePosy = pos.y / gridHeight;
			absPos = ofVec2f((percentagePosx*scrSize.x - parentTrans.absPos.x) / parentTrans.absScale.x,
				(percentagePosy*scrSize.y - parentTrans.absPos.y) / parentTrans.absScale.y);
			break;
		}

		return absPos;
	}

	ofVec3f TransformMath::CalcScale(Node* node, Node* parent, float width, float height, CalcType scaleCalc, int gridWidth, int gridHeight) {
		Vec2i scrSize = CogGetVirtualScreenSize();
		Trans& parentTrans = parent->GetTransform();

		float scaleX = 1;
		float scaleY = 1;

		switch (scaleCalc) {
		case CalcType::ABS:
			// absolute scale
			scaleX = width / parentTrans.absScale.x;
			scaleY = height / parentTrans.absScale.y;
			break;
		case CalcType::LOC:
			// local scale, is multiplied by parent scale
			if (width == 0) width = 1;
			if (height == 0) height = 1;
			scaleX = width;
			scaleY = height;
			break;
		case CalcType::ABS_PER:
			// absolute percentage scale -> 1.0 x 1.0 will fit the whole screen
			scaleX = (width* scrSize.x / node->GetShape()->GetWidth()) / parentTrans.absScale.x;
			scaleY = (height* scrSize.y / node->GetShape()->GetHeight()) / parentTrans.absScale.y;
			break;
		case CalcType::PER:
			// relative percentage scale ->1.0 x 1.0 will fit the whole parent
			if (width == 0) width = 1;
			if (height == 0) height = 1;
			scaleX = (width * parent->GetShape()->GetWidth() / node->GetShape()->GetWidth());
			scaleY = (height * parent->GetShape()->GetHeight() / node->GetShape()->GetHeight());
			break;

		case CalcType::GRID:
			// grid scale -> todo, not tested yet :-)
			float percentageWidth = width / gridWidth;
			float percentageHeight = height / gridHeight;
			scaleX = (percentageWidth* scrSize.x / node->GetShape()->GetWidth()) / parentTrans.absScale.x;
			scaleY = (percentageHeight* scrSize.y / node->GetShape()->GetHeight()) / parentTrans.absScale.y;
			break;
		}

		if (width != 0 && height != 0) return ofVec3f(scaleX, scaleY, 1);
		else if (width == 0) return ofVec3f(scaleY, scaleY, 1);
		else return ofVec3f(scaleX, scaleX, 1); // height == 0

	}

} // namespace