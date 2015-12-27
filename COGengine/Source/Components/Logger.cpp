#include "Logger.h"
#include "CogEngine.h"

namespace Cog {

	/**
	* Initializes component, using xml settings or default one
	*/
	void Logger::Init(spt<ofxXmlSettings> config) {
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
				if (channelStr.compare("console") == 0) channel = new ConsoleLoggerChannel();
				else if (channelStr.compare("file") == 0) {
					bool append = ofToBool(set.GetItemVal("append"));
					string path = set.GetItemVal("path");
					string fullPath = ofToDataPath(path);
					channel = new FileLoggerChannel(fullPath, append);
				}
			}
		}
		else {
			Init();
		}
	}


} // namespace