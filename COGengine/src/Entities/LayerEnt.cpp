#include "LayerEnt.h"
#include "Error.h"

namespace Cog {

	void LayerEnt::LoadFromXml(xml_node& node) {
		
		string sheetName = node.attribute("spritesheet").as_string("");
		string name = node.attribute("name").as_string("");

		if (sheetName.empty()) throw IllegalArgumentException("Layer must have spritesheet attribute specified");

		if (name.empty()) name = sheetName;

		int bufferSize = node.attribute("buffer_size").as_int(10);
		int zIndex = node.attribute("z_index").as_int(0);

		this->name = name;
		this->spriteSheetName = sheetName;
		this->bufferSize = bufferSize;
		this->zIndex = zIndex;
	}

} // namespace