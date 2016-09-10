#pragma once

#include "Component.h"
#include "ofxSQLite.h"
#include "Node.h"

namespace Cog {

	/**
	* Interface for objects stored in the GridSpace
	*/
	class ICellObject {
	public:
		virtual ofVec2f GetPosition() = 0;
		virtual ofVec2f GetPreviousPosition() = 0;
	};
	

	/**
	* Space which represents a grid-based graph divided into smaller pieces, called cells 
	* Granularity is configurable via partitionIndex attribute
	*
	* Separate cells contain collection of all objects, located in that cell; this enables quick queries
	* over the grid space to find nearest objects
	* @tparam class implementing ICellObject interface
	*/
	template <class T>
	class GridSpace {
	private:

		/**
		* Class that holds collection of objects within a single cell
		*/
		class Cell {
		public:
			// objects within a cell
			vector<T*> objects;

			// cell's bounding box
			ofRectangle boundingBox;

			Cell(ofRectangle boundingBox) : boundingBox(boundingBox) {

			}
		};

		// collection of cells
		vector<Cell*> cells;
		// collection of all objects
		vector<T*> allObjects;
		// size of the space
		ofVec2f spaceSize;
		// number of rows
		int rows;
		// number of columns
		int columns;
		// width of one cell
		float cellWidth;
		// height of one cell
		float cellHeight;
		// partition index used while building the space
		// for index = 1, the space will have the same size as the referenced graph
		int partitionIndex;
	public:

		/**
		* Creates a new Grid Space object
		* @param spaceSize size of the space
		* @param partitionIndex granularity index - if equals 1, the grid will have 
		* the same size as the referenced graph
		*/
		GridSpace(ofVec2f spaceSize, int partitionIndex)
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

		int GetRows() const {
			return rows;
		}

		int GetColumns() const {
			return columns;
		}

		float GetCellWidth() const {
			return cellWidth;
		}

		float GetCellHeight() const {
			return cellHeight;
		}

		int GetPartitionIndex() const {
			return partitionIndex;
		}

		void AddObject(T* obj) {
			int pos = CalcIndex(obj->GetPosition());
			cells[pos]->objects.push_back(obj);
			allObjects.push_back(obj);
		}

		void UpdateObjects() {
			for (auto& obj : allObjects) {
				UpdateObject(obj);
			}
		}

		void UpdateObject(T* obj) {
			int oldPos = CalcIndex(obj->GetPreviousPosition());
			int newPos = CalcIndex(obj->GetPosition());

			if (newPos != oldPos) {
				auto& objects = cells[oldPos]->objects;

				for (auto it = objects.begin(); it != objects.end(); ++it) {
					if ((*it) == obj) {
						objects.erase(it);
						break;
					}
				}

				cells[newPos]->objects.push_back(obj);
			}
		}

		void CalcNeighbors(ofVec2f position, float radius, vector<T*>& output) {
			
			ofRectangle queryRect = ofRectangle(position-ofVec2f(radius),position+ofVec2f(radius));

			for (auto it = cells.begin(); it != cells.end(); ++it) {
				Cell* cell = (*it);
				if (cell->objects.size() > 0 && cell->boundingBox.intersects(queryRect)) {
					
					for (auto jt = cell->objects.begin(); jt != cell->objects.end(); ++jt) {
						T* obj = *jt;
						auto nodePos = obj->GetPosition();
						if (position.distanceSquared(nodePos) < radius*radius) {
							output.push_back(obj);
						}
					}
				}
			}
		}

		bool ExistsObject(ofVec2f position, float radius) {
			ofRectangle queryRect = ofRectangle(position - ofVec2f(radius), position + ofVec2f(radius));

			for (auto it = cells.begin(); it != cells.end(); ++it) {
				Cell* cell = (*it);
				if (cell->objects.size() > 0 && cell->boundingBox.intersects(queryRect)) {

					for (auto jt = cell->objects.begin(); jt != cell->objects.end(); ++jt) {
						T* obj = *jt;
						if (obj->GetPosition().distanceSquared(position) < radius*radius) {
							return true;
						}
					}
				}
			}

			return false;
		}

		void ClearCells() {
			for (auto& cell : cells) {
				cell->objects.clear();
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