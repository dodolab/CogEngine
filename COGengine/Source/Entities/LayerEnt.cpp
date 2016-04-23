#include "LayerEnt.h"
#include "Error.h"

namespace Cog {

	void LayerEnt::LoadFromXml(spt<ofxXml> xml) {
		
		string sheetName = xml->getAttributex("spritesheet", "");
		string name = xml->getAttributex("name", "");

		if (sheetName.empty()) throw IllegalArgumentException("Layer must have spritesheet attribute specified");

		if (name.empty()) name = sheetName;

		int bufferSize = xml->getAttributex("buffer_size", 10);
		int zIndex = xml->getAttributex("z_index", 0);

		this->name = name;
		this->spriteSheetName = sheetName;
		this->bufferSize = bufferSize;
		this->zIndex = zIndex;
	}

} // namespace