#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include "ofUtils.h"
#include "MUtils.h"

using namespace std;

/*! Logging level */
enum class LogLevel{
	LERROR = 0,		/*!< log only errors */
	LINFO = 1,		/*!< log errors and info */
	LDEBUG = 2		/*!< log errors, info and debug messages */
};


/**
* Common class for all logger channels
*/
class LoggerChannel{
public:
	virtual ~LoggerChannel(){}

	/**
	* Logs the message for selected level
	* @param level logging level
	* @param depth log depth (padding from left in text output)
	* @param message message to log
	*/
	virtual void Log(LogLevel level, int depth, string message) = 0;

	/**
	* Flushes logged messages
	* e.g. for file channel it saves messages into output file
	*/
	virtual void Flush(){
	}

protected:

	/**
	* Gets log level in string
	*/
	string GetLogLevel(LogLevel level){
		switch (level){
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
class ConsoleLoggerChannel : public LoggerChannel{
public:
	virtual ~ConsoleLoggerChannel(){};

	virtual void Log(LogLevel level, int depth, string message){

		spaces(depth * 2, cout);

		cout << GetLogLevel(level);
		cout << "[" << ofGetTimestampString("%H:%M:%S.%i") << "]";
		cout << message << endl;
	}
};

/**
* Channel that logs to the file
*/
class FileLoggerChannel : public LoggerChannel{
public:
	FileLoggerChannel(const string & path, bool append) : path(path), append(append), anyLog(false){

	}

	virtual ~FileLoggerChannel(){
		st.clear();
	}

	virtual void Log(LogLevel level, int depth, string message){
		spaces(depth * 2, st);

		st << GetLogLevel(level);
		st << "[" << ofGetTimestampString("%H:%M:%S.%i") << "]";
		st << message << endl;

		anyLog = true;
	}

	virtual void Flush(){
		if (anyLog){
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
class MLogger{

protected:
	LoggerChannel* channel;
	LogLevel logLevel;

public:

	/**
	* Creates a new logger
	* @param channel default logging channel
	* @param logLevel lowest level that should bel logged
	*/
	MLogger(LoggerChannel* channel, LogLevel logLevel) : channel(channel), logLevel(logLevel){
	
	}

	~MLogger(){
		delete channel;
	}

	/**
	* Logs error message
	* @param depth log depth (padding from left)
	*/
	void LogError(int depth, const char* format, va_list args){
		if (((int)logLevel) >= ((int)LogLevel::LERROR)){
			channel->Log(LogLevel::LERROR, depth, ofVAArgsToString(format, args));
		}
	}

	/**
	* Logs info message
	* @param depth log depth (padding from left)
	*/
	void LogInfo(int depth, const char* format, va_list args){
		if (((int)logLevel) >= ((int)LogLevel::LINFO)){
			channel->Log(LogLevel::LINFO, depth, ofVAArgsToString(format, args));
		}
	}

	/**
	* Logs debug message
	* @param depth log depth (padding from left)
	*/
	void LogDebug(int depth, const char* format, va_list args){
		if (((int)logLevel) >= ((int)LogLevel::LDEBUG)){
			channel->Log(LogLevel::LDEBUG, depth, ofVAArgsToString(format, args));
		}
	}

	/**
	* Flushes logged messages
	* e.g. for file channel it saves messages into output file
	*/
	void Flush(){
		channel->Flush();
	}

};

