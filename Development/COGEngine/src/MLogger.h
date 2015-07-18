#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include "ofUtils.h"

using namespace std;

enum class LogLevel{
	LERROR, LINFO, LDEBUG
};


class LoggerChannel{
public:
	virtual ~LoggerChannel(){}
	virtual void Log(LogLevel level, int depth, string message) = 0;
	virtual void Flush(){

	}

protected:
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

	void AppendSpaces(int howMany, ostream& ss){
		for (int i = 0; i < howMany; i++){
			ss << " ";
		}
	}
};

class ConsoleLoggerChannel : public LoggerChannel{
public:
	virtual ~ConsoleLoggerChannel(){};
	virtual void Log(LogLevel level, int depth, string message){

		AppendSpaces(depth * 2, cout);

		cout << GetLogLevel(level);
		cout << "[" << ofGetTimestampString("%H:%M:%S.%i") << "]";
		cout << message << endl;
	}
};

class FileLoggerChannel : public LoggerChannel{
public:
	FileLoggerChannel(const string & path, bool append) : path(path), append(append), anyLog(false){

	}

	virtual ~FileLoggerChannel(){
		st.clear();
	}

	virtual void Log(LogLevel level, int depth, string message){
		AppendSpaces(depth * 2, st);

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

private:
	bool anyLog;
	ostringstream st;
	string path;
	bool append;
};




class MLogger{

protected:
	ostringstream st;
	LoggerChannel* channel;

	void Log(LogLevel level, int depth, const char* format, va_list args){
		st << "ERROR: " << ofVAArgsToString(format, args) << endl;
	}

public:

	MLogger(LoggerChannel* channel) : channel(channel){
	
	}

	~MLogger(){
		delete channel;
	}

	void LogError(int depth, const char* format, va_list args){
		channel->Log(LogLevel::LERROR, depth, ofVAArgsToString(format,args));
	}

	void LogInfo(int depth, const char* format, va_list args){
		channel->Log(LogLevel::LINFO, depth, ofVAArgsToString(format, args));
	}

	void LogDebug(int depth, const char* format, va_list args){
		channel->Log(LogLevel::LDEBUG, depth, ofVAArgsToString(format, args));
	}

	void Flush(){
		channel->Flush();
	}

};

