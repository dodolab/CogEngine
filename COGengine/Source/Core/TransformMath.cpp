#include "TransformMath.h"
#include "Node.h"
#include "Settings.h"

namespace Cog {

	void TransformMath::SetSizeToScreen(Node* node, Node* parent) {
		auto trans = TransformEnt("", ofVec2f(0, 0), 1,
			CALCTYPE_PER,
			ofVec2f(0, 0),
			ofVec2f(1, 0), // zero because we want to scale according to the x axis
			CALCTYPE_ABS_PER, 0);

		SetTransform(node, parent, trans, 0, 0);
	}

	void TransformMath::SetTransform(Node* node, Node* parent, TransformEnt& entity, int gridWidth, int gridHeight) {

		Trans nodeTransform = Trans(0, 0);

		CalcTransform(nodeTransform, node, parent, entity, gridWidth, gridHeight);

		auto shape = node->GetMesh();

		// for rectangles, width and height are set directly instead of scale
		switch (shape->GetMeshType()) {
		case MESH_NONE:
		case MESH_RECTANGLE:
		case MESH_BOUNDING_BOX:
			auto rectShape = node->GetMesh<Rectangle>();
			rectShape->SetWidth(rectShape->GetWidth()*nodeTransform.scale.x);
			rectShape->SetHeight(rectShape->GetHeight()*nodeTransform.scale.y);
			nodeTransform.scale = ofVec3f(1);
			break;
		}

		// refresh transform (recalculate from parent)
		nodeTransform.CalcAbsTransform(parent->GetTransform());
		node->SetTransform(nodeTransform);
	}

	void TransformMath::CalcTransform(Trans& outputTrans, Node* node, Node* parent, TransformEnt& entity, int gridWidth, int gridHeight) {

		Trans& parentTrans = parent->GetTransform();

		// calculate scale
		ofVec2f scale = CalcScale(node, parent, entity.size.x, entity.size.y, entity.sType, gridWidth, gridHeight);
		// calculate position
		ofVec2f absPos = CalcPosition(node, parent, entity.pos, entity.pType, gridWidth, gridHeight);

		auto shape = node->GetMesh();

		// fix position according to the anchor
		absPos.x += (0.0f - entity.anchor.x) * shape->GetWidth()*scale.x;
		absPos.y += (0.0f - entity.anchor.y) * shape->GetHeight()*scale.y;

		int zIndex = entity.zIndex;
		// if zIndex is equal to 0, the value will be taken from the parent
		if (zIndex == 0) zIndex = (int)parentTrans.localPos.z;

		// set transformation
		outputTrans.localPos = ofVec3f(absPos.x, absPos.y, (float)zIndex);

		outputTrans.scale = ofVec3f(scale.x, scale.y, 1);
		outputTrans.rotationCentroid = ofVec2f(shape->GetWidth(), shape->GetHeight()) * entity.rotationCentroid * (scale); // multiply by abs scale
		outputTrans.rotation = entity.rotation;
	}

	ofVec2f TransformMath::CalcPosition(Node* node, Node* parent, ofVec2f pos, stenum posCalc, int gridWidth, int gridHeight) {
		Vec2i scrSize = CogGetVirtualScreenSize();
		Trans& parentTrans = parent->GetTransform();
		ofVec2f absPos;


		switch (posCalc) {
		case CALCTYPE_ABS:
			// absolute position in pixels
			absPos = ofVec2f((pos.x - parentTrans.absPos.x) / parentTrans.absScale.x,
				(pos.y - parentTrans.absPos.y) / parentTrans.absScale.y);
			break;
		case CALCTYPE_LOC:
			// local position is scaled according to the absolute scale of the parent
			absPos = pos;
			break;
		case CALCTYPE_ABS_PER:
			// absolute percentage -> screen size is 1.0 x 1.0
			absPos = ofVec2f((pos.x*scrSize.x - parentTrans.absPos.x) / parentTrans.absScale.x,
				(pos.y*scrSize.y - parentTrans.absPos.y) / parentTrans.absScale.y);
			break;
		case CALCTYPE_PER:
			// relative percentage -> parent size is 1.0 x 1.0
			absPos = ofVec2f(pos.x*parent->GetMesh()->GetWidth(),
				pos.y*parent->GetMesh()->GetHeight());
			break;
		case CALCTYPE_GRID:
			// grid percentage -> grid size must be specified
			float percentagePosx = pos.x / gridWidth;
			float percentagePosy = pos.y / gridHeight;
			absPos = ofVec2f((percentagePosx*scrSize.x - parentTrans.absPos.x) / parentTrans.absScale.x,
				(percentagePosy*scrSize.y - parentTrans.absPos.y) / parentTrans.absScale.y);
			break;
		}

		return absPos;
	}

	ofVec3f TransformMath::CalcScale(Node* node, Node* parent, float width, float height, stenum scaleCalc, int gridWidth, int gridHeight) {
		Vec2i scrSize = CogGetVirtualScreenSize();
		Trans& parentTrans = parent->GetTransform();

		float scaleX = 1;
		float scaleY = 1;

		switch (scaleCalc) {
		case CALCTYPE_ABS:
			// absolute scale
			scaleX = width / parentTrans.absScale.x;
			scaleY = height / parentTrans.absScale.y;
			break;
		case CALCTYPE_LOC:
			// local scale, multiplied by scale of the parent
			if (width == 0) width = 1;
			if (height == 0) height = 1;
			scaleX = width;
			scaleY = height;
			break;
		case CALCTYPE_ABS_PER:
			// absolute percentage scale -> 1.0 x 1.0 will fit the whole screen
			scaleX = (width* scrSize.x / node->GetMesh()->GetWidth()) / parentTrans.absScale.x;
			scaleY = (height* scrSize.y / node->GetMesh()->GetHeight()) / parentTrans.absScale.y;
			break;
		case CALCTYPE_PER:
			// relative percentage scale ->1.0 x 1.0 will fit the whole parent
			scaleX = (width * parent->GetMesh()->GetWidth() / node->GetMesh()->GetWidth());
			scaleY = (height * parent->GetMesh()->GetHeight() / node->GetMesh()->GetHeight());
			break;

		case CALCTYPE_GRID:
			// grid scale 
			float percentageWidth = width / gridWidth;
			float percentageHeight = height / gridHeight;
			scaleX = (percentageWidth* scrSize.x / node->GetMesh()->GetWidth()) / parentTrans.absScale.x;
			scaleY = (percentageHeight* scrSize.y / node->GetMesh()->GetHeight()) / parentTrans.absScale.y;
			break;
		}

		// correct auto size
		if (width != 0 && height != 0) return ofVec3f(scaleX, scaleY, 1);
		else if (width == 0) return ofVec3f(scaleY, scaleY, 1);
		else return ofVec3f(scaleX, scaleX, 1); // height = 0

	}

	ofVec2f TransformMath::GetCenter() {
		return ofVec2f(CogGetScreenWidth() / 2.0f, CogGetScreenHeight() / 2.0f);
	}

} // namespace