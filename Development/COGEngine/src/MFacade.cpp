
#include "MFacade.h"
#include "MGameEngine.h"
#include "MLogger.h"

void MFLogError(string message){
	MEngine.logger->LogError(message);
}

void MFLogInfo(string message){
	MEngine.logger->LogInfo(message);
}

void MFLogDebug(string message){
	MEngine.logger->LogDebug(message);
}

void MFLoggerSave(){
	MEngine.logger->Save();
}