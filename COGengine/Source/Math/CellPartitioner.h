#pragma once

#include "Component.h"
#include "ofxSQLite.h"
#include "Node.h"

namespace Cog {

	/**
	* Class that holds collection of nodes of one cell
	*/
	class Cell {
	public:
		// nodes in the cell
		vector<Node*> nodes;

		// cell's bounding box
		ofRectangle boundingBox;

		Cell(ofRectangle boundingBox) : boundingBox(boundingBox) {

		}
	};

	/**
	* Class that holds collection of cells
	*/
	class CellSpace {
		vector<Cell*> cells;
		map<int, ofVec2f> nodesAndPositions;

		ofVec2f spaceSize;
		int rows;
		int columns;
		float cellWidth;
		float cellHeight;
		int partitionIndex;
	public:

		CellSpace(ofVec2f spaceSize, int partitionIndex)
		:spaceSize(spaceSize), partitionIndex(partitionIndex){

			this->columns = spaceSize.x / partitionIndex;
			this->rows = spaceSize.y / partitionIndex;

			this->cellWidth = spaceSize.x / columns;
			this->cellHeight = spaceSize.y / rows;

			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < columns; j++) {
					ofRectangle boundingBox = ofRectangle(j*cellWidth,i*cellHeight,cellWidth,cellHeight);
					cells.push_back(new Cell(boundingBox));
				}
			}
		}

		void AddNode(Node* node) {
			int pos = CalcIndex(node->GetTransform().localPos);
			cells[pos]->nodes.push_back(node);

			nodesAndPositions[node->GetId()] = node->GetTransform().localPos;
		}

		void UpdateNode(Node* node) {
			ofVec2f previousPos = nodesAndPositions[node->GetId()];
			UpdateNode(node, previousPos);
		}

		void UpdateNode(Node* node, ofVec2f previousPos) {
			int oldPos = CalcIndex(previousPos);
			int newPos = CalcIndex(node->GetTransform().localPos);

			if (newPos != oldPos) {
				auto& nodes = cells[oldPos]->nodes;

				for (auto it = nodes.begin(); it != nodes.end(); ++it) {
					if ((*it)->GetId() == node->GetId()) {
						nodes.erase(it);
						break;
					}
				}

				cells[newPos]->nodes.push_back(node);
			}

			nodesAndPositions[node->GetId()] = node->GetTransform().localPos;
		}

		vector<Node*> CalcNeighbors(ofVec2f position, float radius) {
			auto neighbors = vector<Node*>();

			ofRectangle queryRect = ofRectangle(position-ofVec2f(radius),position+ofVec2f(radius));

			for (auto& it = cells.begin(); it != cells.end(); ++it) {
				Cell* cell = (*it);
				if (cell->nodes.size() > 0 && cell->boundingBox.intersects(queryRect)) {
					
					for (auto& jt = cell->nodes.begin(); jt != cell->nodes.end(); ++jt) {
						Node* node = *jt;
						auto nodePos = node->GetTransform().localPos;
						if (position.distanceSquared(nodePos) < radius*radius) {
							neighbors.push_back(node);
						}
					}
				}
			}

			return neighbors;
		}

		bool ExistsNode(ofVec2f position, float radius) {
			ofRectangle queryRect = ofRectangle(position - ofVec2f(radius), position + ofVec2f(radius));

			for (auto& it = cells.begin(); it != cells.end(); ++it) {
				Cell* cell = (*it);
				if (cell->nodes.size() > 0 && cell->boundingBox.intersects(queryRect)) {

					for (auto& jt = cell->nodes.begin(); jt != cell->nodes.end(); ++jt) {
						Node* node = *jt;
						if (node->GetTransform().localPos.distanceSquared(position) < radius*radius) {
							return true;
						}
					}
				}
			}

			return false;
		}

		void ClearCells() {
			for (auto& cell : cells) {
				cell->nodes.clear();
			}
		}

		int CalcIndex(ofVec2f position) {

			int posX = (int)(columns * position.x/spaceSize.x);
			int posY = (int)(rows * position.y/spaceSize.y);
			int pos = posY*columns + posX;

			if (pos > (int)cells.size() - 1) pos = (int)cells.size() - 1;

			return pos;
		}

	};


}// namespace