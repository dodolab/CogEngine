#include "Logger.h"
#include "CogEngine.h"
#include "Settings.h"
#include "ResourceCache.h"
#include "EntityStorage.h"

namespace Cog {


	/**
	* Initializes component, using xml settings or default one
	*/
	void Logger::OnInit(spt<ofxXmlSettings> config) {
		delete channel;
		channel = nullptr;

		// use global settings that is already loaded
		auto resCache = GETCOMPONENT(ResourceCache);
		Setting set = resCache->GetGlobalSettings("logger");

		if (!set.name.empty()) {
			string level = ofToLower(set.GetItemVal("level"));
			if (!level.empty()) {
				if (level.compare("error") == 0) logLevel = LogLevel::LERROR;
				else if (level.compare("info") == 0) logLevel = LogLevel::LINFO;
				else if (level.compare("debug") == 0) logLevel = LogLevel::LDEBUG;
			}

			string channelStr = ofToLower(set.GetItemVal("channel"));
			if (!channelStr.empty()) {
				if (channelStr.compare("console") == 0) SetOutputToConsole();
				else if (channelStr.compare("file") == 0) {
					bool append = set.GetItemValBool("append",false);
					string path = set.GetItemVal("path");
					string fullPath = ofToDataPath(path);
					SetOutputToFile(fullPath, append);
				}
			}

			// load includes and excludes
			this->includes = set.GetItemVals("module_include");;
			this->excludes = set.GetItemVals("module_exclude");;
		}
		else {
			OnInit();
		}
	}


} // namespace