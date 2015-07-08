#pragma once

#include <string>

using namespace std;

// facade methods for M objects

// =================== MLOGGER =========================

void MFLogError(string message);
void MFLogInfo(string message);
void MFLogDebug(string message);
void MFLoggerSave();