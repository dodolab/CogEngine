#pragma once

#include "Component.h"
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
		map<Vec2i, Cell*> cells;
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
		}

		/**
		* Gets number of rows in the grid space
		*/
		int GetRows() const {
			return rows;
		}

		/**
		* Gets number of columns in the grid space
		*/
		int GetColumns() const {
			return columns;
		}

		/**
		* Gets width of one cell
		*/
		float GetCellWidth() const {
			return cellWidth;
		}

		/**
		* Gets height of one cell
		*/
		float GetCellHeight() const {
			return cellHeight;
		}

		/**
		* Gets partition index (granularity of the grid space)
		*/
		int GetPartitionIndex() const {
			return partitionIndex;
		}

		/**
		* Inserts a new object
		*/
		void AddObject(T* obj) {
			Vec2i pos = CalcPos(obj->GetPosition());
			GetCell(pos)->objects.push_back(obj);
			allObjects.push_back(obj);
		}

		/**
		* Updates all stored objects
		*/
		void UpdateObjects() {
			for (auto& obj : allObjects) {
				UpdateObject(obj);
			}
		}

		/**
		* Updates selected object
		*/
		void UpdateObject(T* obj) {
			Vec2i oldPos = CalcPos(obj->GetPreviousPosition());
			Vec2i newPos = CalcPos(obj->GetPosition());

			if (newPos != oldPos) {
				auto cell = GetCell(oldPos);
				auto& objects = cell->objects;

				for (auto it = objects.begin(); it != objects.end(); ++it) {
					if ((*it) == obj) {
						objects.erase(it);
						// remove cell with no objects
						if (cell->objects.empty()) cells.erase(oldPos);
						break;
					}
				}

				GetCell(newPos)->objects.push_back(obj);
			}
		}

		/**
		* Obtains objects at selected position in given radius
		* @param position selected position where to search
		* @param radius distance from position where to search for objects
		* @param output output collection of found objects
		*/
		void CalcNeighbors(ofVec2f position, float radius, vector<T*>& output) {
			
			ofRectangle queryRect = ofRectangle(position-ofVec2f(radius),position+ofVec2f(radius));

			for (auto it = cells.begin(); it != cells.end(); ++it) {
				Cell* cell = (*it).second;
				if (cell->objects.size() > 0 && cell->boundingBox.intersects(queryRect)) {
					
					for (auto jt = cell->objects.begin(); jt != cell->objects.end(); ++jt) {
						T* obj = *jt;
						auto nodePos = obj->GetPosition();
						if (position.distanceSquared(nodePos) <= radius*radius) {
							output.push_back(obj);
						}
					}
				}
			}
		}

		/**
		* Returns true, if there is at least one object at selected position
		* @param position selected position where to search
		* @param radius distance from position where to search for objects
		*/
		bool ExistsObject(ofVec2f position, float radius) {
			ofRectangle queryRect = ofRectangle(position - ofVec2f(radius), position + ofVec2f(radius));

			for (auto it = cells.begin(); it != cells.end(); ++it) {
				Cell* cell = (*it).second;
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

		/**
		* Clears objects from all cells
		*/
		void ClearCells() {
			for (auto& cell : cells) {
				cell.second->objects.clear();
			}
		}

protected:

		/**
		* Calculates position in grid space in accordance with real position
		*/
		Vec2i CalcPos(ofVec2f position) {
			int posX = (int)(columns * position.x/spaceSize.x);
			int posY = (int)(rows * position.y/spaceSize.y);
			return Vec2i(posX, posY);
		}

		/**
		* Gets cell at selected position or creates a new one
		*/
		Cell* GetCell(Vec2i pos) {
			if (cells.find(pos) == cells.end()) {
				cells[pos] = new Cell(ofRectangle(pos.x*cellWidth, pos.y*cellHeight, cellWidth, cellHeight));
			}

			return cells[pos];
		}

	};


}// namespace