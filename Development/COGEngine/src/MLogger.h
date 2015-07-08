#pragma once
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

class MLogger{

protected:
	ostringstream st;

public:

	MLogger(){

	}

	void LogError(string message){
		st << "ERROR: " << message << endl;
	}

	void LogInfo(string message){
		st << "INFO: " << message << endl;
	}

	void LogDebug(string message){
		st << "DEBUG: " << message << endl;
	}

	void Save(){

	}

};

