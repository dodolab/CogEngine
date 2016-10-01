#pragma once

#include "Component.h"

namespace Cog {

	/**
	* Provides access to custom asset files and XML manifests
	*/
	class AssetsManager {
	private:
		map<string, string> strings;
		map<string, string> dimensions;
		map<string, int> integers;
	public:

		void OnInit();

		/**
		* Gets string by its key
		*/
		string GetString(string key);

		/**
		* Gets dimension by its key
		*/
		string GetDimension(string key);

		/**
		* Gets integer by its key
		*/
		int GetInteger(string key);
	};

} // namespace