#pragma once

#include "GridSpace.h"
#include "Node.h"

namespace Cog {

	/**
	* Implementation of ICellObject for Nodes
	*/
	class NodeCellObject : public ICellObject{
	public:
		Node* node;
		ofVec2f previousPos;
		ofVec2f position;

		NodeCellObject(Node* node) : node(node) {
			previousPos = node->GetTransform().localPos;
			position = previousPos;
		}

		virtual ofVec2f GetPosition() {
			UpdatePositions();
			return position;
		}

		virtual ofVec2f GetPreviousPosition() {
			UpdatePositions();
			return previousPos;
		}

	protected:
		void UpdatePositions() {
			auto actualPos = node->GetTransform().localPos;
			if (actualPos != position) {
				previousPos = position;
				position = actualPos;
			}
		}
	};

} // namespace