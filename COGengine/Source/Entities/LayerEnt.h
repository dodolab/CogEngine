#pragma once

#include "ofxCogCommon.h"
#include "DEntity.h"

namespace Cog {

	/**
	* Entity for layer settings
	*/
	class LayerEnt : public DEntity {
	public:
		// name of the sprite sheet this layer represents
		string spriteSheetName;
		// buffer size of the layer
		int bufferSize;
		// lowest z-index of the layer (z-indices of all objects
		// inside a layer are restricted by this index
		int zIndex;

		LayerEnt() {

		}

		/**
		* Creates a new layer entity
		* @param name name of the layer
		* @param spriteSheetName name of sprite sheet this layer represents
		* @param bufferSize size of the buffer of this layer
		* @param zindex z-index of the layer 
		*/
		LayerEnt(string name, string spriteSheetName, int bufferSize, int zindex) : spriteSheetName(spriteSheetName), bufferSize(bufferSize), zIndex(zIndex){
			this->name = name;
		}

		~LayerEnt() {

		}

		/**
		* Loads entity from XML
		*/
		void LoadFromXml(spt<ofxXml> xml);

	};
}// namespace


