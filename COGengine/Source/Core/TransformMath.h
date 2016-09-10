#pragma once

#include "ofxCogMain.h"

namespace Cog {

	/**
	* Transformation helper
	*/
	class TransformMath  {

	private:


	public:
		void SetSizeToScreen(Node* node, Node* parent) {
			SetTransform(node, parent, ofVec2f(0, 0), 1, CalcType::PER, ofVec2f(0, 0), ofVec2f(1, 0) // zero because we want to scale according to the x axis
				, CalcType::PER, 0, 0);
		}

		void SetTransform(Node* node, Node* parent, ofVec2f position, int zIndex, CalcType positionCalc, ofVec2f anchor, ofVec2f size, CalcType sizeCalc, int gridWidth = 0, int gridHeight = 0) {
			ofVec2f scrSize = CogGetScreenSize();

			Trans& parentTrans = parent->GetTransform();
			Trans nodeTransform = Trans(0, 0);

			ofVec2f scale = CalcScale(node, parent, size.x, size.y, sizeCalc, gridWidth, gridHeight);
			ofVec2f absPos = CalcPosition(node, parent, position, positionCalc, gridWidth, gridHeight);

			absPos.x += (0.5f - anchor.x) * node->GetShape()->GetWidth()*scale.x;
			absPos.y += (0.5f - anchor.y) * node->GetShape()->GetHeight()*scale.y;

			nodeTransform.localPos = ofVec3f(absPos.x, absPos.y, node->GetTransform().localPos.z);
			nodeTransform.scale = ofVec3f(scale.x, scale.y, 1);

			// refresh transform
			nodeTransform.CalcAbsTransform(parent->GetTransform());

			node->SetTransform(nodeTransform);
		}

		ofVec2f GetCenter() {
			return ofVec2f(CogGetScreenWidth() / 2, CogGetScreenHeight() / 2);
		}


		ofVec2f CalcPosition(Node* node, Node* parent, ofVec2f pos, CalcType posCalc, int gridWidth = 0, int gridHeight = 0) {
			ofVec2f scrSize = CogGetScreenSize();
			Trans& parentTrans = parent->GetTransform();
			ofVec2f absPos;

			switch (posCalc) {
			case CalcType::ABS:
				absPos = ofVec2f((pos.x - parentTrans.absPos.x) / parentTrans.absScale.x, 
					(pos.y - parentTrans.absPos.y) / parentTrans.absScale.y);
				break;
			case CalcType::LOC:
				absPos = pos;
				break;
			case CalcType::ABS_PER:
				absPos = ofVec2f((pos.x*scrSize.x - parentTrans.absPos.x) / parentTrans.absScale.x,
					(pos.y*scrSize.y - parentTrans.absPos.y) / parentTrans.absScale.y);
				break;
			case CalcType::PER:
				

				absPos = ofVec2f(2*(-0.5f+pos.x)*parent->GetShape()->GetWidth() * parentTrans.absScale.x, 
					2*(-0.5f+pos.y)*parent->GetShape()->GetHeight()*parentTrans.absScale.y);
				break;
			case CalcType::GRID:
				float percentagePosx = pos.x / gridWidth;
				float percentagePosy = pos.y / gridHeight;
				absPos = ofVec2f((percentagePosx*scrSize.x - parentTrans.absPos.x) / parentTrans.absScale.x, 
					(percentagePosy*scrSize.y - parentTrans.absPos.y) / parentTrans.absScale.y);
				break;
			}

			return absPos;
		}


		ofVec3f CalcScale(Node* node, Node* parent, float width, float height, CalcType scaleCalc, int gridWidth = 0, int gridHeight = 0) {
			ofVec2f scrSize = CogGetScreenSize();
			Trans& parentTrans = parent->GetTransform();

			float scaleX = 1;
			float scaleY = 1;

			switch (scaleCalc) {
			case CalcType::ABS:
				scaleX = width / parentTrans.absScale.x;
				scaleY = height / parentTrans.absScale.y;
				break;
			case CalcType::LOC:
				if (width == 0) width = 1;
				if (height == 0) height = 1;
				scaleX = width;
				scaleY = height;
				break;
			case CalcType::ABS_PER:
				scaleX = (width* scrSize.x / node->GetShape()->GetWidth()) / parentTrans.absScale.x;
				scaleY = (height* scrSize.y / node->GetShape()->GetHeight()) / parentTrans.absScale.y;
				break;
			case CalcType::PER:
				if (width == 0) width = 1;
				if (height == 0) height = 1;
				scaleX = (width* scrSize.x / node->GetShape()->GetWidth());
				scaleY = (height* scrSize.y / node->GetShape()->GetHeight());
				break;
			case CalcType::GRID:
				float percentageWidth = width / gridWidth;
				float percentageHeight = height / gridHeight;
				scaleX = (percentageWidth* scrSize.x / node->GetShape()->GetWidth()) / parentTrans.absScale.x;
				scaleY = (percentageHeight* scrSize.y / node->GetShape()->GetHeight()) / parentTrans.absScale.y;
				break;
			}

			if (width != 0 && height != 0) return ofVec3f(scaleX, scaleY, 1);
			else if (width == 0) return ofVec3f(scaleY, scaleY, 1);
			else if (height == 0)return ofVec3f(scaleX, scaleX, 1);

		}


	};

}// namespace