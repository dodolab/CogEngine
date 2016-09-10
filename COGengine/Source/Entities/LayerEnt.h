#pragma once

#include "ofxCogCommon.h"
#include "DEntity.h"

namespace Cog {

	/**
	* Entity for layer settings
	*/
	class LayerEnt : public DEntity {
	public:
		string spriteSheetName;
		int bufferSize;
		int zIndex;

		LayerEnt() {

		}

		LayerEnt(string name, string spriteSheetName, int bufferSize, int zindex) : spriteSheetName(spriteSheetName), bufferSize(bufferSize), zIndex(zIndex){
			this->name = name;
		}

		~LayerEnt() {

		}

		void LoadFromXml(spt<ofxXml> xml) {
			string sheetName = xml->getAttributex("spritesheet", "");
			string name = xml->getAttributex("name", "");

			if (sheetName.empty()) throw IllegalArgumentException("spritesheet attribute not specified for scene layer");
			
			if (name.empty()) name = sheetName;
			
			int bufferSize = xml->getAttributex("buffer_size", 10);
			int zIndex = xml->getAttributex("z_index", 0);
			
			this->name = name;
			this->spriteSheetName = sheetName;
			this->bufferSize = bufferSize;
			this->zIndex = zIndex;
		}

	};
}// namespace


