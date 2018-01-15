#include "Logger.h"
#include "ofxCogEngine.h"
#include "Settings.h"
#include "ResourcesMgr.h"
#include "ComponentStorage.h"
#include "Facade.h"

namespace Cog {


	void ConsoleLogger::Log(LogLevel level, const char* module, string message, int depth) {

		spaces(depth * 2, cout);

#ifdef WIN32
		cout << GetLogLevel(level);
		cout << "[" << ofGetTimestampString("%H:%M:%S.%i") << "]";
		cout << "[" << module << "]";
		cout << message << endl;
#else
		ofLogNotice("COGEngine") << "[" << ofGetTimestampString("%H:%M:%S.%i") << "]" << "[" << module << "]" << message << endl;
#endif
	}

	void FileLogger::Log(LogLevel level, const char* module, string message, int depth) {
		spaces(depth * 2, st);

		st << GetLogLevel(level);
		st << "[" << ofGetTimestampString("%H:%M:%S.%i") << "]";
		st << "[" << module << "]";
		st << message << endl;

		anyLog = true;
	}

	void FileLogger::Flush() {
		if (anyLog) {
			ofFile file;
			file.open(path, append ? ofFile::Append : ofFile::WriteOnly);
			file << st.str();
			file.close();

			// clear
			st.str(std::string());
			anyLog = false;
		}
	}

	void Logger::OnInit(xml_node& config) {
		delete logOutput;
		logOutput = nullptr;

		auto resCache = GETCOMPONENT(Resources);
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

	void Logger::LogError(const char* module, int depth, const char* format, va_list args) {
		if (((int)logLevel) >= ((int)LogLevel::LERROR)) {
			logOutput->Log(LogLevel::LERROR, module, ofVAArgsToString(format, args), depth);
		}
	}

	void Logger::LogInfo(const char* module, int depth, const char* format, va_list args) {

		if ((includes.empty() || find(includes.begin(), includes.end(), string(module)) != includes.end()) &&
			(excludes.empty() || find(excludes.begin(), excludes.end(), string(module)) == excludes.end())) {

			if (((int)logLevel) >= ((int)LogLevel::LINFO)) {
				logOutput->Log(LogLevel::LINFO, module, ofVAArgsToString(format, args), depth);
			}
		}
	}

	void Logger::LogDebug(const char* module, int depth, const char* format, va_list args) {

		if ((includes.empty() || find(includes.begin(), includes.end(), string(module)) != includes.end()) &&
			(excludes.empty() || find(excludes.begin(), excludes.end(), string(module)) == excludes.end())) {

			if (((int)logLevel) >= ((int)LogLevel::LDEBUG)) {
				logOutput->Log(LogLevel::LDEBUG, module, ofVAArgsToString(format, args), depth);
			}
		}
	}

	void Logger::Update(const uint64 delta, const uint64 absolute) {
		// flush each 4 second
		if (CogGetFrameCounter() % 240 == 0) {
			Flush();
		}
	}

} // namespace