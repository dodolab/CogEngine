#pragma once

#include "ofxCogCommon.h"
#include "ofUtils.h"
#include "Component.h"

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

#ifdef WIN32
			cout << GetLogLevel(level);
			cout << "[" << ofGetTimestampString("%H:%M:%S.%i") << "]";
			cout << "[" << module << "]";
			cout << message << endl;
#else
			ofLogNotice("OpenFrameworks") << "[" << ofGetTimestampString("%H:%M:%S.%i") << "]" << "[" << module << "]" << message << endl;
#endif
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

		OBJECT(Logger)

	protected:
		LoggerChannel* channel;
		LogLevel logLevel;
		spt<ofxXmlSettings> config;
		vector<string> includes;
		vector<string> excludes;

	public:

		/**
		* Creates a new logger
		* @param config configuration xml
		*/
		Logger() {
			initPriority = InitPriority::MEDIUM; // logger will be initialized before environment
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
		void Init(spt<ofxXmlSettings> config);
	

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

			if ((includes.empty() || find(includes.begin(), includes.end(), string(module)) != includes.end()) &&
				(excludes.empty() || find(excludes.begin(), excludes.end(), string(module)) == excludes.end())) {

				if (((int)logLevel) >= ((int)LogLevel::LINFO)) {
					channel->Log(LogLevel::LINFO, module, ofVAArgsToString(format, args), depth);
				}
			}
		}

		/**
		* Logs debug message
		* @param depth log depth (padding from left)
		*/
		void LogDebug(const char* module, int depth, const char* format, va_list args) {

			if ((includes.empty() || find(includes.begin(), includes.end(), string(module)) != includes.end()) &&
				(excludes.empty() || find(excludes.begin(), excludes.end(), string(module)) == excludes.end())) {

				if (((int)logLevel) >= ((int)LogLevel::LDEBUG)) {
					channel->Log(LogLevel::LDEBUG, module, ofVAArgsToString(format, args), depth);
				}
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