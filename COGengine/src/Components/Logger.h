#pragma once

#include <sstream>

#include "ofUtils.h"
#include "Definitions.h"
#include "Component.h"

namespace Cog {

	/** Logging level */
	enum class LogLevel {
		LERROR = 0,		/** log only errors */
		LINFO = 1,		/** log errors and info */
		LDEBUG = 2		/** log errors, info and debug messages */
	};


	/**
	* Base class for all types of logger
	*/
	class LoggerOutput {
	public:
		virtual ~LoggerOutput() {}

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
		* @param depth logging depth (text indent)
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
	* Logger that logs to the console
	*/
	class ConsoleLogger : public LoggerOutput {
	public:
		virtual ~ConsoleLogger() {};

		virtual void Log(LogLevel level, const char* module, string message, int depth);
	};

	/**
	* Logger that logs to the file
	*/
	class FileLogger : public LoggerOutput {
	public:
		FileLogger(const string & path, bool append) : path(path), append(append), anyLog(false) {

		}

		virtual ~FileLogger() {
			st.clear();
		}

		virtual void Log(LogLevel level, const char* module, string message, int depth);

		virtual void Flush();

	protected:
		// indicator whether there are some new messages
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

	protected:
		LoggerOutput* logOutput = nullptr;
		LogLevel logLevel;
		xml_node config;
		// collection of modules to include
		vector<string> includes;
		// collection of modules to exclude
		vector<string> excludes;

	public:

		/**
		* Creates a new logger
		*/
		Logger() {
			initPriority = InitPriority::MEDIUM; // logger will be initialized before environment
		}

		~Logger() {
			delete logOutput;
		}

		/**
		* Gets collection of included modules
		* If there is neither module to include nor to exclude,
		* logger will log everything
		*
		* If there is at least one module to include, the logger will
		* log only this module
		*/
		vector<string>& GetIncludedModules() {
			return includes;
		}

		/**
		* Gets collection of excluded modules
		* If there is neither module to include nor to exclude,
		* logger will log everything
		*
		* If there are some excluded modules but no included modules,
		* logger will log everything except the excluded modules
		*/
		vector<string>& GetExcludedModules() {
			return excludes;
		}

		void OnInit() {
			delete logOutput;
			logOutput = new ConsoleLogger();
			logLevel = LogLevel::LINFO;
		}

		void OnInit(xml_node& config);
	

		/**
		* Logs error message
		* @param module source module
		* @param depth log depth (text indent)
		* @param format log message
		*/
		void LogError(const char* module, int depth, const char* format, va_list args);

		/**
		* Logs info message
		* @param module source module
		* @param depth log depth (text indent)
		* @param format log message
		*/
		void LogInfo(const char* module, int depth, const char* format, va_list args);

		/**
		* Logs debug message
		* @param module source module
		* @param depth log depth (text indent)
		* @param format log message
		*/
		void LogDebug(const char* module, int depth, const char* format, va_list args);

		/**
		* Flushes logged messages
		*/
		void Flush() {
			logOutput->Flush();
		}

		/**
		* Sets log level
		*/
		void SetLogLevel(LogLevel logLevel) {
			this->logLevel = logLevel;
		}

		/**
		* Sets output to the console
		*/
		void SetOutputToConsole() {
			delete logOutput;
			logOutput = new ConsoleLogger();
		}

		/**
		* Sets output to the file
		*/
		void SetOutputToFile(string path, bool append) {
			delete logOutput;
			logOutput = new FileLogger(path, append);
		}

		virtual void Update(const uint64 delta, const uint64 absolute);
	};

}// namespace