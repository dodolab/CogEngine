#pragma once

#include "ofxCogMain.h"
#include "ofUtils.h"
#include "Utils.h"


namespace Cog {

	/*! Logging level */
	enum class LogLevel {
		LERROR = 0,		/*!< log only errors */
		LINFO = 1,		/*!< log errors and info */
		LDEBUG = 2		/*!< log errors, info and debug messages */
	};


	/**
	* Common class for all logger channels
	*/
	class LoggerChannel {
	public:
		virtual ~LoggerChannel() {}

		/**
		* Logs the message for selected level
		* @param level logging level
		* @param module source module
		* @param message message to log
		*/
		void Log(LogLevel level, const char* module, string message) {
			this->Log(level, module, message, 0);
		}

		/**
		* Logs the message for selected level
		* @param level logging level
		* @param module source module
		* @param message message to log
		* @param depth logging depth (provided by text padding)
		*/
		virtual void Log(LogLevel level, const char* module, string message, int depth) = 0;

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
		string GetLogLevel(LogLevel level) {
			switch (level) {
			case LogLevel::LERROR:
				return "[ERROR]";
			case LogLevel::LINFO:
				return "[INFO]";
			case LogLevel::LDEBUG:
				return "[DEBUG]";
			default:
				return "";
			}
		}

	};

	/**
	* Channel that logs to the console
	*/
	class ConsoleLoggerChannel : public LoggerChannel {
	public:
		virtual ~ConsoleLoggerChannel() {};

		virtual void Log(LogLevel level, const char* module, string message, int depth) {

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
	class FileLoggerChannel : public LoggerChannel {
	public:
		FileLoggerChannel(const string & path, bool append) : path(path), append(append), anyLog(false) {

		}

		virtual ~FileLoggerChannel() {
			st.clear();
		}

		virtual void Log(LogLevel level, const char* module, string message, int depth) {
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
	class Logger : public Component {

		COMPONENT(Logger)

	protected:
		LoggerChannel* channel;
		LogLevel logLevel;
		spt<ofxXmlSettings> config;

	public:
		REGISTER_XMLHANDLING("logger")

			/**
			* Creates a new logger
			* @param config configuration xml
			*/
			Logger() {
		}

		~Logger() {
			delete channel;
		}

		void Init() {
			channel = new ConsoleLoggerChannel();
			logLevel = LogLevel::LERROR;
		}

		/**
		* Initializes component, using xml settings or default one
		*/
		void Init(spt<ofxXmlSettings> config) {
			if (config->tagExists("logger")) {
				config->pushTag("logger");

				string level = ofToLower(config->getValue("level", "info"));

				if (level.compare("error") == 0) logLevel = LogLevel::LERROR;
				else if (level.compare("info") == 0) logLevel = LogLevel::LINFO;
				else if (level.compare("debug") == 0) logLevel = LogLevel::LDEBUG;

				if (config->tagExists("channel")) {
					string type = config->getAttribute("channel", "type", "console");

					if (type.compare("console") == 0) channel = new ConsoleLoggerChannel();
					else if (type.compare("file") == 0) {
						bool append = config->getBoolAttribute("channel", "append", true);
						string path = config->getAttribute("channel", "path", "");
						string fullPath = ofToDataPath(path);
						channel = new FileLoggerChannel(fullPath, append);
					}

				}
				else channel = new ConsoleLoggerChannel();


				config->popTag();
			}
		}
	

		/**
		* Logs error message
		* @param depth log depth (padding from left)
		*/
		void LogError(const char* module, int depth, const char* format, va_list args) {
			if (((int)logLevel) >= ((int)LogLevel::LERROR)) {
				channel->Log(LogLevel::LERROR, module, ofVAArgsToString(format, args), depth);
			}
		}

		/**
		* Logs info message
		* @param depth log depth (padding from left)
		*/
		void LogInfo(const char* module, int depth, const char* format, va_list args) {
			if (((int)logLevel) >= ((int)LogLevel::LINFO)) {
				channel->Log(LogLevel::LINFO, module, ofVAArgsToString(format, args), depth);
			}
		}

		/**
		* Logs debug message
		* @param depth log depth (padding from left)
		*/
		void LogDebug(const char* module, int depth, const char* format, va_list args) {
			if (((int)logLevel) >= ((int)LogLevel::LDEBUG)) {
				channel->Log(LogLevel::LDEBUG, module, ofVAArgsToString(format, args), depth);
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

}// namespace