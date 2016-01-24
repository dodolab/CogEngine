#pragma once

#include "ofxCogCommon.h"
#include "Component.h"
#include "Anim.h"
#include "Settings.h"

namespace Cog {

	/**
	* Simple configuration loader
	* Parses non-xml text files
	*/
	class ConfigLoader {

	public:

		/**
		* Reads configuration file of format  key=value, separated with newline character
		*/
		Setting ReadKeyValueConfig(string filename) {
			Setting config = Setting();

			config.name = filename;

			ofBuffer buffer = ofBufferFromFile(filename);

			if (buffer.size()) {

				// parse lines
				for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
					string line = *it;
					size_t pos = 0;
					pos = line.find("=");
					string key = line.substr(0, pos);
					line.erase(0, pos + 1);
					string value = line;

					SettingItem sit = SettingItem();
					sit.key = key;
					sit.values.push_back(value);

					config.items[key] = sit;
				}
			}

			return config;
		}

	};

} // namespace