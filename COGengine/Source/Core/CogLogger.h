#pragma once

#include "ofxCogMain.h"
#include "ofUtils.h"
#include "CogUtils.h"


namespace Cog {

	/*! Logging level */
	enum class CogLogLevel {
		LERROR = 0,		/*!< log only errors */
		LINFO = 1,		/*!< log errors and info */
		LDEBUG = 2		/*!< log errors, info and debug messages */
	};


	/**
	* Common class for all logger channels
	*/
	class CogLoggerChannel {
	public:
		virtual ~CogLoggerChannel() {}

		/**
		* Logs the message for selected level
		* @param level logging level
		* @param module source module
		* @param message message to log
		*/
		void Log(CogLogLevel level, const char* module, string message) {
			this->Log(level, module, message, 0);
		}

		/**
		* Logs the message for selected level
		* @param level logging level
		* @param module source module
		* @param message message to log
		* @param depth logging depth (provided by text padding)
		*/
		virtual void Log(CogLogLevel level, const char* module, string message, int depth) = 0;

		/**
		* Flushes logged messages
		* e.g. for file channel it saves messages into output file
		*/
		virtual void Flush() {
		}

	protected:

		/**
		* Gets log level in string
		*/
		string GetLogLevel(CogLogLevel level) {
			switch (level) {
			case CogLogLevel::LERROR:
				return "[ERROR]";
			case CogLogLevel::LINFO:
				return "[INFO]";
			case CogLogLevel::LDEBUG:
				return "[DEBUG]";
			default:
				return "";
			}
		}

	};

	/**
	* Channel that logs to the console
	*/
	class CogConsoleLoggerChannel : public CogLoggerChannel {
	public:
		virtual ~CogConsoleLoggerChannel() {};

		virtual void Log(CogLogLevel level, const char* module, string message, int depth) {

			spaces(depth * 2, cout);

			cout << GetLogLevel(level);
			cout << "[" << ofGetTimestampString("%H:%M:%S.%i") << "]";
			cout << "[" << module << "]";
			cout << message << endl;
		}
	};

	/**
	* Channel that logs to the file
	*/
	class CogFileLoggerChannel : public CogLoggerChannel {
	public:
		CogFileLoggerChannel(const string & path, bool append) : path(path), append(append), anyLog(false) {

		}

		virtual ~CogFileLoggerChannel() {
			st.clear();
		}

		virtual void Log(CogLogLevel level, const char* module, string message, int depth) {
			spaces(depth * 2, st);

			st << GetLogLevel(level);
			st << "[" << ofGetTimestampString("%H:%M:%S.%i") << "]";
			st << "[" << module << "]";
			st << message << endl;

			anyLog = true;
		}

		virtual void Flush() {
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

	protected:
		// indicator if some message has been appended
		bool anyLog;
		ostringstream st;
		// path to log file
		string path;
		// if true, new messages will be appended to already existing file
		bool append;
	};


	/**
	* Log controller
	*/
	class CogLogger {

	protected:
		CogLoggerChannel* channel;
		CogLogLevel logLevel;
		spt<ofxXmlSettings> config;

	public:

		/**
		* Creates a new logger
		* @param config configuration xml
		*/
		CogLogger(spt<ofxXmlSettings> config) : config(config) {
		}

		~CogLogger() {
			delete channel;
		}

		/**
		* Initializes component, using xml settings or default one
		*/
		void Init() {
			if (config != spt<ofxXmlSettings>()) {
				if (config->tagExists("logger")) {
					config->pushTag("logger");

					string level = ofToLower(config->getValue("level", "info"));

					if (level.compare("error") == 0) logLevel = CogLogLevel::LERROR;
					else if (level.compare("info") == 0) logLevel = CogLogLevel::LINFO;
					else if (level.compare("debug") == 0) logLevel = CogLogLevel::LDEBUG;

					if (config->tagExists("channel")) {
						string type = config->getAttribute("channel", "type", "console");

						if (type.compare("console") == 0) channel = new CogConsoleLoggerChannel();
						else if (type.compare("file") == 0) {
							bool append = config->getBoolAttribute("channel", "append", true);
							string path = config->getAttribute("channel", "path", "");
							string fullPath = ofToDataPath(path);
							channel = new CogFileLoggerChannel(fullPath, append);
						}

					}
					else channel = new CogConsoleLoggerChannel();


					config->popTag();
				}
			}
			else {
				// no config available, use default settings
				channel = new CogConsoleLoggerChannel();
				logLevel = CogLogLevel::LERROR;
			}
		}

		/**
		* Logs error message
		* @param depth log depth (padding from left)
		*/
		void LogError(const char* module, int depth, const char* format, va_list args) {
			if (((int)logLevel) >= ((int)CogLogLevel::LERROR)) {
				channel->Log(CogLogLevel::LERROR, module, ofVAArgsToString(format, args), depth);
			}
		}

		/**
		* Logs info message
		* @param depth log depth (padding from left)
		*/
		void LogInfo(const char* module, int depth, const char* format, va_list args) {
			if (((int)logLevel) >= ((int)CogLogLevel::LINFO)) {
				channel->Log(CogLogLevel::LINFO, module, ofVAArgsToString(format, args), depth);
			}
		}

		/**
		* Logs debug message
		* @param depth log depth (padding from left)
		*/
		void LogDebug(const char* module, int depth, const char* format, va_list args) {
			if (((int)logLevel) >= ((int)CogLogLevel::LDEBUG)) {
				channel->Log(CogLogLevel::LDEBUG, module, ofVAArgsToString(format, args), depth);
			}
		}

		/**
		* Flushes logged messages
		* e.g. for file channel it saves messages into output file
		*/
		void Flush() {
			channel->Flush();
		}

	};

}