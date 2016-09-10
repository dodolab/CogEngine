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
		vector<Node*> neighbors;
		vector<Node*>::iterator currentNeighbor;

		ofVec2f spaceSize;
		int rows;
		int columns;
		float cellWidth;
		float cellHeight;
		int partitionIndex;
	public:

		CellSpace(ofVec2f spaceSize, int rows, int columns, int partitionIndex)
		:spaceSize(spaceSize), rows(rows), columns(columns), partitionIndex(partitionIndex){
			this->cellWidth = spaceSize.x / columns;
			this->cellHeight = spaceSize.y / columns;

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
		}

		void CalcNeighbors(ofVec2f position, float radius) {
			neighbors.clear();

			auto currentNeighbor = neighbors.begin();

			ofRectangle queryRect = ofRectangle(position-ofVec2f(radius),position+ofVec2f(radius));

			for (auto& it = cells.begin(); it != cells.end(); ++it) {
				Cell* cell = (*it);
				if (cell->boundingBox.inside(queryRect) && cell->nodes.size() > 0) {
					
					for (auto& jt = cell->nodes.begin(); jt != cell->nodes.end(); ++it) {
						Node* node = *jt;
						if (node->GetTransform().localPos.distanceSquared(position) < radius*radius) {
							neighbors.push_back(node);
						}
						
					}
				}
			}
		}

		Node* GetPreviousNeighbor() {
			currentNeighbor--;
			return *currentNeighbor;
		}

		Node* GetNextNeighbor() {
			currentNeighbor++;
			return *currentNeighbor;
		}

		bool HasNextNeighbor() {
			return currentNeighbor != neighbors.end();
		}

		void ClearCells() {
			for (auto& cell : cells) {
				cell->nodes.clear();
			}
		}

		int CalcIndex(ofVec2f position) {
			int pos = (int)(columns * position.x / spaceSize.x) +
				((int)((rows)* position.y / spaceSize.y) * columns);

			if (pos > (int)cells.size() - 1) pos = (int)cells.size() - 1;

			return pos;
		}

	};

	/**
	* Class that divides scene into a grid of cells so that
	* fast searching querys can be made
	*/
	class CellPartitioner : public Component{
		OBJECT(CellPartitioner)
	
	private:
		
	public:

		~CellPartitioner() {
			
		}

		void Init() {

		}

		void Init(spt<ofxXml> xml) {

		}

	
	};

}// namespace